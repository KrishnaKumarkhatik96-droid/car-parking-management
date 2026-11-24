#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdio>
using namespace std;

const int TOTAL_SLOTS = 30;
const int PARKING_RATE = 20;

struct ParkingRecord
{
    int token;
    string vehicle;
    string driver;
    string mobile;
    int slot;
    string entryDate;
};

vector<ParkingRecord> readActiveRecords()
{
    vector<ParkingRecord> records;
    ifstream file("data.text");

    ParkingRecord r;
    while (file >> r.token >> r.vehicle >> r.driver >> r.mobile >> r.slot)
    {
        getline(file, r.entryDate);
        records.push_back(r);
    }

    return records;
}

bool saveActiveRecords(const vector<ParkingRecord>& records)
{
    ofstream file("data.text");
    if (!file.is_open())
        return false;

    for (size_t i = 0; i < records.size(); i++)
    {
        file << records[i].token << " "
             << records[i].vehicle << " "
             << records[i].driver << " "
             << records[i].mobile << " "
             << records[i].slot
             << records[i].entryDate << endl;
    }

    return true;
}

int main()
{
    int choice;
    string vehicleNumber, mobileNumber, DriverName;

    do
    {
        cout << endl;
        cout << "===== CAR PARKING SYSTEM =====" << endl;
        cout << "1. Vehicle Entry" << endl;
        cout << "2. Vehicle Exit" << endl;
        cout << "3. Change Parking Slot" << endl;
        cout << "4. Exit program" << endl;
        cout << "Total Parking Slots: " << TOTAL_SLOTS << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            cout << "Enter Driver Name: ";
            cin.ignore();
            getline(cin, DriverName);

            for (size_t i = 0; i < DriverName.length(); i++)
            {
                if (DriverName[i] == ' ')
                    DriverName[i] = '_';
            }

            cout << "Enter Mobile Number: ";
            cin >> mobileNumber;

            vector<ParkingRecord> records = readActiveRecords();
            bool occupied[TOTAL_SLOTS + 1] = {false};
            bool alreadyParked = false;
            int existingSlot = 0, existingToken = 0;
            int tokenNumber = 1000;

            for (size_t i = 0; i < records.size(); i++)
            {
                if (records[i].slot >= 1 && records[i].slot <= TOTAL_SLOTS)
                    occupied[records[i].slot] = true;

                if (records[i].token > tokenNumber)
                    tokenNumber = records[i].token;

                if (records[i].vehicle == vehicleNumber)
                {
                    alreadyParked = true;
                    existingSlot = records[i].slot;
                    existingToken = records[i].token;
                }
            }

            if (alreadyParked)
            {
                cout << "Ye vehicle pehle se parked hai!" << endl;
                cout << "Existing Slot: " << existingSlot << endl;
                cout << "Existing Token: P-" << existingToken << endl;
                continue;
            }

            int assignedSlot = 0;
            for (int i = 1; i <= TOTAL_SLOTS; i++)
            {
                if (!occupied[i])
                {
                    assignedSlot = i;
                    break;
                }
            }

            if (assignedSlot == 0)
            {
                cout << "Parking Full" << endl;
                continue;
            }

            cout << "Assigned Slot: " << assignedSlot << endl;

            int actualSlot;
            cout << "Enter the slot where car is parked: ";
            cin >> actualSlot;

            while (actualSlot != assignedSlot)
            {
                cout << "WRONG PARKING - Please move the car!" << endl;
                cout << "Re-enter the correct slot: ";
                cin >> actualSlot;
            }

            cout << "RIGHT PARKING - PARKING CONFIRMED!" << endl;
            cout << "STATUS = PARKED" << endl;

            tokenNumber++;
            cout << "Token ID: P-" << tokenNumber << endl;

            time_t now = time(0);
            char* entryTime = ctime(&now);
            cout << "Entry Date & Time: " << entryTime;

            ofstream saveFile("data.text", ios::app);
            if (saveFile.is_open())
            {
                saveFile << tokenNumber << " "
                         << vehicleNumber << " "
                         << DriverName << " "
                         << mobileNumber << " "
                         << assignedSlot << " "
                         << entryTime;
                saveFile.close();
                cout << "Entry data saved successfully!" << endl;
            }
            else
            {
                cout << "File open nahi hui!" << endl;
            }
        }
        else if (choice == 2)
        {
            int searchToken;
            cout << endl << "===== VEHICLE EXIT =====" << endl;
            cout << "Enter Token Number: P-";
            cin >> searchToken;

            vector<ParkingRecord> records = readActiveRecords();
            int index = -1;

            for (size_t i = 0; i < records.size(); i++)
            {
                if (records[i].token == searchToken)
                {
                    index = static_cast<int>(i);
                    break;
                }
            }

            if (index == -1)
            {
                cout << "Token not found!" << endl;
                continue;
            }

            ParkingRecord r = records[index];

            if (!r.entryDate.empty() && r.entryDate[0] == ' ')
                r.entryDate.erase(0, 1);

            cout << "Vehicle Number: " << r.vehicle << endl;
            cout << "Driver Name: " << r.driver << endl;
            cout << "Mobile Number: " << r.mobile << endl;
            cout << "Slot Number: " << r.slot << endl;
            cout << "Entry Time: " << r.entryDate << endl;

            tm entryTM = {};
            istringstream timeStream(r.entryDate);
            timeStream >> get_time(&entryTM, "%a %b %d %H:%M:%S %Y");

            if (timeStream.fail())
            {
                cout << "Entry time read nahi ho paya. Exit cancel kar diya gaya." << endl;
                continue;
            }

            time_t entryTime = mktime(&entryTM);
            time_t exitTime = time(0);
            char* exitTimeText = ctime(&exitTime);
            string exitTimeString = exitTimeText;

            if (!exitTimeString.empty() && exitTimeString[exitTimeString.length() - 1] == '\n')
                exitTimeString.erase(exitTimeString.length() - 1);

            cout << "Exit Time: " << exitTimeString << endl;

            double durationSeconds = difftime(exitTime, entryTime);
            int durationMinutes = static_cast<int>(durationSeconds / 60);
            int hours = durationMinutes / 60;
            int minutes = durationMinutes % 60;
            int chargedHours = (durationMinutes + 59) / 60;

            if (chargedHours == 0)
                chargedHours = 1;

            int fee = chargedHours * PARKING_RATE;

            cout << "Duration: " << hours << " hour(s) " << minutes << " minute(s)" << endl;
            cout << "Parking Fee: Rs. " << fee << endl;

            int paymentChoice;
            do
            {
                cout << endl << "==== PAYMENT ====" << endl;
                cout << "1. Confirm Payment" << endl;
                cout << "2. Cancel Payment" << endl;
                cout << "Enter your choice: ";
                cin >> paymentChoice;

                if (paymentChoice == 1)
                    cout << "Payment Successful!" << endl;
                else if (paymentChoice == 2)
                    cout << "Payment Cancelled! Please try payment again." << endl;
                else
                    cout << "Invalid choice!" << endl;
            }
            while (paymentChoice != 1);

            ofstream historyFile("exit.text", ios::app);
            if (historyFile.is_open())
            {
                historyFile << r.token << " " << r.vehicle << " "
                            << r.driver << " " << r.mobile << " " << r.slot
                            << " Entry Time:" << r.entryDate
                            << " Exit Time:" << exitTimeString
                            << " Duration: " << hours << "h " << minutes
                            << "m Fee: " << fee << endl;
                historyFile.close();
                cout << "Exit history saved successfully!" << endl;
            }

            records.erase(records.begin() + index);
            if (saveActiveRecords(records))
            {
                cout << "Vehicle removed from active parking!" << endl;
                cout << "Slot " << r.slot << " is now AVAILABLE." << endl;
                cout << "EXIT CONFIRMED!" << endl;
                cout << "Thank you, have a nice day!" << endl;
            }
            else
            {
                cout << "Active parking file update nahi ho payi!" << endl;
            }
        }
        else if (choice == 3)
        {
            int searchToken, newSlot;
            cout << endl << "===== CHANGE PARKING SLOT =====" << endl;
            cout << "Enter Token Number: P-";
            cin >> searchToken;

            vector<ParkingRecord> records = readActiveRecords();
            int index = -1;

            for (size_t i = 0; i < records.size(); i++)
            {
                if (records[i].token == searchToken)
                {
                    index = static_cast<int>(i);
                    break;
                }
            }

            if (index == -1)
            {
                cout << "Token not found! Vehicle active parking mein nahi hai." << endl;
                continue;
            }

            cout << "Vehicle: " << records[index].vehicle << endl;
            cout << "Current Slot: " << records[index].slot << endl;
            cout << "Enter New Slot (1-" << TOTAL_SLOTS << "): ";
            cin >> newSlot;

            if (newSlot < 1 || newSlot > TOTAL_SLOTS)
            {
                cout << "Invalid slot! Slot 1 se " << TOTAL_SLOTS << " ke beech hona chahiye." << endl;
                continue;
            }

            if (newSlot == records[index].slot)
            {
                cout << "Vehicle already isi slot mein hai." << endl;
                continue;
            }

            bool slotOccupied = false;
            for (size_t i = 0; i < records.size(); i++)
            {
                if (records[i].slot == newSlot)
                {
                    slotOccupied = true;
                    break;
                }
            }

            if (slotOccupied)
            {
                cout << "Slot " << newSlot << " already occupied hai. Dusra slot choose karo." << endl;
                continue;
            }

            int oldSlot = records[index].slot;
            records[index].slot = newSlot;

            if (saveActiveRecords(records))
            {
                cout << "Slot successfully changed!" << endl;
                cout << "Vehicle: " << records[index].vehicle << endl;
                cout << "Old Slot: " << oldSlot << endl;
                cout << "New Slot: " << newSlot << endl;
            }
            else
            {
                cout << "Slot update nahi ho paya. File check karo." << endl;
            }
        }
        else if (choice == 4)
        {
            cout << "Program closed. Thank you, have a nice day." << endl;
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 4);

    return 0;
}
