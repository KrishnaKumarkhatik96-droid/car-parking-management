#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

int main()
{
    int choice;

    string vehicleNumber;
    string mobileNumber;
    string DriverName;

    do
    {
        cout << endl;
        cout << "===== CAR PARKING SYSTEM =====" << endl;
        cout << "1. Vehicle Entry" << endl;
        cout << "2. Vehicle Exit" << endl;
        cout << "3. Exit program" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            cout << "Enter Driver Name: ";
            cin.ignore();
            getline(cin, DriverName);

            for (int i = 0; i < DriverName.length(); i++)
            {
                if (DriverName[i] == ' ')
                    DriverName[i] = '_';
            }

            cout << "Enter Mobile Number: ";
            cin >> mobileNumber;

            cout << endl;

            cout << "Vehicle Number: " << vehicleNumber << endl;
            cout << "Driver name: " << DriverName << endl;
            cout << "Mobile Number: " << mobileNumber << endl;

            int totalSlots = 3;
            int assignedSlot = 0;
            int tokenNumber = 1000;

            bool occupied[4] = {false};
            bool alreadyParked = false;
            int existingSlot = 0, existingToken = 0;

            ifstream file;
            file.open("data.text");

            int oldToken, oldSlot;
            string oldVehicle, oldDriver, oldMobile;

            while (file >> oldToken >> oldVehicle >> oldDriver >> oldMobile >> oldSlot)
            {
                if (oldSlot >= 1 && oldSlot <= totalSlots)
                {
                    occupied[oldSlot] = true;
                }

                if (oldToken > tokenNumber)
                    tokenNumber = oldToken;

                if (oldVehicle == vehicleNumber)
                {
                    alreadyParked = true;
                    existingSlot = oldSlot;
                    existingToken = oldToken;
                }

                string oldDate;
                getline(file, oldDate);
            }

            file.close();

            if (alreadyParked)
            {
                cout << endl;
                cout << "Ye vehicle pehle se parked hai!" << endl;
                cout << "Existing Slot: " << existingSlot << endl;
                cout << "Existing Token: P-" << existingToken << endl;
                continue;
            }

            tokenNumber = tokenNumber + 1;

            for (int i = 1; i <= totalSlots; i++)
            {
                if (occupied[i] == false)
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

            cout << "RIGHT PARKING - PARKING CONFIRMED! " << endl;
            cout << "STATUS = PARKED" << endl;

            cout << "Token ID: P-" << tokenNumber << endl;

            time_t now = time(0);
            char* entryTime = ctime(&now);

            cout << "Entry Date & Time: " << entryTime;

            ofstream saveFile;
            saveFile.open("data.text", ios::app);

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

            cout << endl;
            cout << "===== VEHICLE EXIT =====" << endl;
            cout << "Enter Token Number: P-";
            cin >> searchToken;

            ifstream exitFile;
            exitFile.open("data.text");

            int exitToken, exitSlot;
            string exitVehicle, exitDriverName, exitMobileNumber;
            string entryDate;

            bool found = false;

            while (exitFile >> exitToken >> exitVehicle >> exitDriverName
                            >> exitMobileNumber >> exitSlot)
            {
                getline(exitFile, entryDate);

                if (exitToken == searchToken)
                {
                    found = true;

                    if (entryDate.length() > 0 && entryDate[0] == ' ')
                        entryDate.erase(0, 1);

                    cout << "Vehicle Number: " << exitVehicle << endl;
                    cout << "Driver Name: " << exitDriverName << endl;
                    cout << "Mobile Number: " << exitMobileNumber << endl;
                    cout << "Slot Number: " << exitSlot << endl;
                    cout << "Entry Time: " << entryDate << endl;

                    tm entryTM = {};
                    istringstream timeStream(entryDate);

                    timeStream >> get_time(&entryTM, "%a %b %d %H:%M:%S %Y");

                    time_t entryTime = mktime(&entryTM);

                    time_t exitTime = time(0);

                    char* exitTimeText = ctime(&exitTime);

                    string exitTimeString = exitTimeText;

                    if (!exitTimeString.empty() && exitTimeString[exitTimeString.length() - 1] == '\n')
                    {
                        exitTimeString.erase(exitTimeString.length() - 1);
                    }
                    cout << "Exit Time: " << exitTimeString << endl;

                    double durationSeconds = difftime(exitTime, entryTime);

                    int durationMinutes = durationSeconds / 60;

                    int hours = durationMinutes / 60;
                    int minutes = durationMinutes % 60;

                    cout << "Duration: " << hours
                         << " hour(s) " << minutes
                         << " minute(s)" << endl;

                    int chargedHours = (durationMinutes + 59) / 60;

                    if (chargedHours == 0)
                        chargedHours = 1;

                    int fee = chargedHours * 20;

                    cout << "Parking Fee: Rs. " << fee << endl;

                    int paymentChoice;
                    do
                    {
                        cout << endl;
                        cout << "==== PAYMENT ====" << endl;
                        cout << "1. Confirm Payment" << endl;
                        cout << "2. Cancel Payment" << endl;
                        cout << "Enter your choice: ";
                        cin >> paymentChoice;

                        if (paymentChoice == 1)
                        {
                            cout << "Payment Successful!" << endl;
                        }
                        else if (paymentChoice == 2)
                        {
                            cout << "Payment Cancelled!" << endl;
                            cout << "Please try payment again." << endl;
                        }
                        else
                        {
                            cout << "Invalid choice!" << endl;
                        }
                    }
                    while (paymentChoice != 1);

                    ofstream historyFile;
                    historyFile.open("exit.text", ios::app);

                    if (historyFile.is_open())
                    {
                        historyFile << exitToken << " " << exitVehicle << " " << exitDriverName << " "
                                    << exitMobileNumber << " " << exitSlot << " " << "Entry Time:" << entryDate << " "
                                    << "Exit Time:" << exitTimeString << " "
                                    << "Duration: " << hours << "h " << minutes << "m " << "Fee: " << fee << endl;

                        historyFile.close();

                        cout << "Exit history saved successfully!" << endl;
                    }
                    exitFile.close();

                    ifstream oldFile;
                    oldFile.open("data.text");

                    ofstream tempFile;
                    tempFile.open("temp.text");

                    int tempToken, tempSlot;
                    string tempVehicle, tempDriver, tempMobile;
                    string tempDate;

                    while (oldFile >> tempToken >> tempVehicle
                                   >> tempDriver >> tempMobile >> tempSlot)
                    {
                        getline(oldFile, tempDate);

                        if (tempToken != searchToken)
                        {
                            tempFile << tempToken << " "
                                     << tempVehicle << " "
                                     << tempDriver << " "
                                     << tempMobile << " "
                                     << tempSlot << " "
                                     << tempDate << endl;
                        }
                    }

                    oldFile.close();
                    tempFile.close();

                    remove("data.text");
                    rename("temp.text", "data.text");

                    cout << "Vehicle removed from active parking!" << endl;
                    cout << "Slot " << exitSlot << " is now AVAILABLE." << endl;
                    cout << "EXIT CONFIRMED!" << endl;
                    cout << "Thank you have nice day:" << endl;

                    break;
                }
            }

            if (!found)
            {
                exitFile.close();
                cout << "Token not found!" << endl;
            }
        }
        else if (choice == 3)
        {
            cout << "Program closed. Thank you, have a nice day." << endl;
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }

    } while (choice != 3);

    return 0;
}