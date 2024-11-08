#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

class Room {
private:
    int roomNumber;
    string roomType;
    double pricePerNight;
    bool isBooked;

public:

    Room(int number = 0, string type = "", double price = 0.0, bool booked = false)
        : roomNumber(number), roomType(type), pricePerNight(price), isBooked(booked) {}

    int getRoomNumber() const 
    {
        return roomNumber;
    }

    string getRoomType() const 
    {
        return roomType;
    }

    double getPricePerNight() const 
    {
        return pricePerNight;
    }

    bool checkAvailability() const 
    {
        return !isBooked;
    }

    void bookRoom()
    {
        if (!isBooked) 
        {
            isBooked = true;
            cout << "Room " << roomNumber << " booked successfully.\n";
        }
        else 
        {
            cout << "Room " << roomNumber << " is already booked.\n";
        }
    }

    void releaseRoom()
    {
        if (isBooked) 
        {
            isBooked = false;
            cout << "Room " << roomNumber << " is now available.\n";
        }
        else 
        {
            cout << "Room " << roomNumber << " is already available.\n";
        }
    }

    void displayInfo() const 
    {
        cout << "| " << setw(10) << left << roomNumber
             << "| " << setw(15) << left << roomType
             << "| $" << setw(9) << left << pricePerNight
             << "| " << setw(10) << left << (isBooked ? "Booked" : "Available")
            << " |" << endl;
    }

    void saveToFile(ofstream& outFile) const 
    {
        outFile << roomNumber << " " << roomType << " " << pricePerNight << " " << isBooked << endl;
    }
    //Access to th private data instead of setter   
    friend ifstream& operator>>(ifstream& inFile, Room& room)
    {
        inFile >> room.roomNumber >> room.roomType >> room.pricePerNight >> room.isBooked;
        return inFile;
    }
};

class Guest {
private:
    string name;
    string nationalID; 
    int roomNumber;

public:
    Guest(string guestName = "", string natID = "")
        : name(guestName), nationalID(natID), roomNumber(-1) {}

    string getName() const 
    {
        return name;
    }

    string getNationalID() const 
    {
        return nationalID; 
    }

    int getRoomNumber() const 
    {
        return roomNumber;
    }

    void checkIn(int roomNum) 
    {
        roomNumber = roomNum;
        clearScreen();
    }

    void checkOut() 
    {
        clearScreen();
        roomNumber = -1;
    }

    void saveToFile(ofstream& outFile) const 
    {
        outFile << name << " " << nationalID << " " << roomNumber << endl; 
    }
    
    friend ifstream& operator>>(ifstream& inFile, Guest& guest) 
    {
        inFile >> guest.name >> guest.nationalID >> guest.roomNumber; 
        return inFile;
    }
};

class Hotel 
{
public:
    vector<Room> rooms;
    vector<Guest> guests;

    void addRoom(const Room& room) 
    {
        rooms.push_back(room);
    }

    void addGuest() 
    {
        string name;
        string nationalID;

        cout << "Enter guest name (or press 0 to go back): ";
        cin >> name;

        if (name == "0") return;
        cout << "Enter guest national ID (or press 0 to go back): ";
        cin >> nationalID;

        if (nationalID == "0") return;
        cout << "\nAvailable Rooms:\n";
        cout << "-------------------------------------------------------" << endl;
        cout << "| " << setw(10) << left << "Room No"
             << "| " << setw(15) << left << "Type"
             << "| " << setw(10) << left << "Price"
             << "| " << setw(10) << left << "Status" << " |" << endl;
        cout << "-------------------------------------------------------" << endl;
        vector<int> availableRoomNumbers;
        for (const auto& room : rooms) //Display the rooms in the hotel
        {
            if (room.checkAvailability()) 
            {
                room.displayInfo();
                availableRoomNumbers.push_back(room.getRoomNumber());
            }
        }
        cout << "-------------------------------------------------------" << endl;
        if (availableRoomNumbers.empty()) //when the all room is booked this message will appear  
        {
            cout << "No available rooms at the moment. Cannot add guest.\n";
            return;
        }
        int roomNumber;
        bool validRoom = false;
        while (!validRoom) //while loop to check the room is booked or not
        { 
            cout << "\nEnter the room number for the guest (or press 0 to go back): ";
            while (true)
            {
                cin >> roomNumber;
                if (cin.fail()) 
                {
                    cin.clear();
                    cin.ignore();
                    cout << "Invalid input! Please enter an available room " << endl;
                    continue;
                }
                else
                {
                    break;
                }
            }
            if (roomNumber == 0) return;
            for (auto& room : rooms) 
            {
                if (room.getRoomNumber() == roomNumber)
                {
                    clearScreen();
                    if (room.checkAvailability()) //if the room available then book the room
                    {
                        validRoom = true;
                        room.bookRoom();
                        Guest newGuest(name, nationalID);
                        newGuest.checkIn(roomNumber);
                        guests.push_back(newGuest);
                        cout << "Guest " << name << " added successfully and checked into room " << roomNumber << ".\n";
                    }
                    else 
                    {
                        cout << "Room " << roomNumber << " is already booked. Please enter another room number.\n";
                    }
                    break;
                }
            }

            if (!validRoom) //if the user input room not exist
            {
                cout << "Unexisted room number. Please enter a valid available room number.\n";
            }
        }
    }

    void releaseRoomFromNumber(int roomNumber)
    { 
        for (auto& room : rooms) 
        {
            if (room.getRoomNumber() == roomNumber) 
            {
                if (!room.checkAvailability()) 
                {
                    room.releaseRoom();
                    for (auto it = guests.begin(); it != guests.end(); ++it) 
                    {
                        if (it->getRoomNumber() == roomNumber) 
                        {
                            it->checkOut();
                            cout << "Guest " << it->getName() << " checked out from room " << roomNumber << ".\n";
                            guests.erase(it); 
                            return;
                        }
                    }
                }
                else
                {
                    cout << "Room " << roomNumber << " is already available.\n";
                }
                return;
            }
        }
        cout << "Room " << roomNumber << " not found.\n";
    }

    void displayRooms() const 
    {
        if (rooms.empty()) 
        {
            cout << "No rooms available.\n";
        }
        else 
        {
            cout << "-------------------------------------------------------" << endl;
            cout << "| " << setw(10) << left << "Room No"
                 << "| " << setw(15) << left << "Type"
                 << "| " << setw(10) << left << "Price"
                 << "| " << setw(10) << left << "Status" << " |" << endl;
            cout << "-------------------------------------------------------" << endl;
            for (const auto& room : rooms)
            {
                room.displayInfo();
            }
            cout << "-------------------------------------------------------" << endl;
        }
    }

    void displayGuests() const 
    {
        if (guests.empty()) 
        {
            cout << "No guests available.\n";
        }
        else 
        {
            cout << "-----------------------------------------------------" << endl;
            cout << "| " << setw(20) << left << "Guest Name"
                 << "| " << setw(15) << left << "National ID"
                 << "| " << setw(10) << left << "Room No" << " |" << endl;
            cout << "-----------------------------------------------------" << endl;
            for (const auto& guest : guests)
            {
                cout << "| " << setw(20) << left << guest.getName()
                     << "| " << setw(15) << left << guest.getNationalID()
                     << "| " << setw(10) << left
                    << (guest.getRoomNumber() == -1 ? "None" : to_string(guest.getRoomNumber()))
                    << " |" << endl;
            }
            cout << "-----------------------------------------------------" << endl;
        }
    }

    void loadData()
    {
        ifstream roomsFile("rooms.txt");
        ifstream guestsFile("guests.txt");
        if (roomsFile.is_open()) 
        {
            Room room;
            while (roomsFile >> room)
            {
                rooms.push_back(room);
            }
            roomsFile.close();
        }
        if (guestsFile.is_open()) 
        {
            Guest guest;
            while (guestsFile >> guest)
            {
                guests.push_back(guest);
            }
            guestsFile.close();
        }
    }

    void saveData()
    {
        ofstream roomsFile("rooms.txt");
        ofstream guestsFile("guests.txt");
        for (const auto& room : rooms)
        {
            room.saveToFile(roomsFile);
        }
        for (const auto& guest : guests) 
        {
            guest.saveToFile(guestsFile);
        }
        roomsFile.close();
        guestsFile.close();
    }
};

void showMenu()
{
    cout << "\n--------- Hotel Management System ---------\n";
    cout << "1. Add Room\n";
    cout << "2. Add Guest\n";
    cout << "3. Check-out Guest\n"; 
    cout << "4. Display All Rooms\n";
    cout << "5. Display All Guests\n";
    cout << "6. Save\n";
    cout << "7. Save and Exit\n";
    cout << "-------------------------------------------\n";
    cout << "Enter your choice: ";
}

int main()
{
    Hotel myHotel;
    myHotel.loadData();
    int choice;

    while (true)
    {
        showMenu();
        cin >> choice;
        cin.ignore();
        if (cin.fail())
        {
            cin.clear();
            cin.ignore();
            clearScreen();
            cout << "Invalid input! Please enter a number between 1 and 8" << endl;
            continue;
        }
        if (choice == 0) 
        {
            clearScreen();
            continue;  
        }
        else if (choice == 1)
        {
            int roomNumber;
            int roomTypeChoice;
            string roomType;
            double price;

            cout << "Enter room number (or press 0 to go back): ";
            cin >> roomNumber;
            if (roomNumber == 0) continue;

            cout << "Select room type:\n";
            cout << "1. Standard\n";
            cout << "2. Duplex\n";
            cout << "3. Suite\n";
            cout << "4. Connecting Rooms\n";
            cout << "Enter your choice (1-4): ";
            while (true)
            {
                cin >> roomTypeChoice;
                if (cin.fail()) 
                {
                   cin.clear();
                   cin.ignore();
                   cout << "Invalid input! Please enter a number between 1 and 4" << endl;
                   continue;
                }
                else
                {
                    break;
                }
            }
            switch (roomTypeChoice)
            {
            case 1:
                roomType = "Standard";
                break;
            case 2:
                roomType = "Duplex";
                break;
            case 3:
                roomType = "Suite";
                break;
            case 4:
                roomType = "Connecting Rooms";
                break;
            default:
                cout << "Invalid choice. Going back to menu.\n";
                continue;
            }
            cout << "Enter price per night: ";
            cin >> price;
            while (true)
            {
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore();
                    cout << "Invalid input! Please enter a valid price.\n";
                    continue;
                }
                else
                {
                    break;
                }
            }
            myHotel.addRoom(Room(roomNumber, roomType, price));
            cout << "Room added successfully.\n";
        }
        else if (choice == 2) 
        {
            myHotel.addGuest();
        }

        else if (choice == 3) 
        {
            int roomNumber;
            cout << "Enter room number to check out (or press 0 to go back): "; 
            cin >> roomNumber;
            if (roomNumber == 0) continue;
            myHotel.releaseRoomFromNumber(roomNumber); 
        }
        else if (choice == 4) 
        {
            clearScreen();
            myHotel.displayRooms();
        }
        else if (choice == 5) 
        {
            clearScreen();
            myHotel.displayGuests();
        }
        else if (choice == 6) 
        {
            myHotel.saveData();
            clearScreen();
            cout << "Data saved\n";
        }
        else if (choice == 7)
        {
            myHotel.saveData();
            clearScreen();
            cout << "Data saved. Exiting...\n";
            break;
        }
        else 
        {
            clearScreen();
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}