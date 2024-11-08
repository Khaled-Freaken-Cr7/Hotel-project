
class Room {
private:
    int roomNumber;
    string roomType;
    double pricePerNight;
    bool isBooked;

public:

    Room(int number = 0, string type = "", double price = 0.0, bool booked = false)
        : roomNumber(number), roomType(type), pricePerNight(price), isBooked(booked) {}

    int getRoomNumber() const {
        return roomNumber;
    }

    string getRoomType() const {
        return roomType;
    }

    double getPricePerNight() const {
        return pricePerNight;
    }

    bool checkAvailability() const {
        return !isBooked;
    }

    void bookRoom() {
        if (!isBooked) {
            isBooked = true;
            cout << "Room " << roomNumber << " booked successfully.\n";
        }
        else {
            cout << "Room " << roomNumber << " is already booked.\n";
        }
    }

    void releaseRoom() {
        if (isBooked) {
            isBooked = false;
            cout << "Room " << roomNumber << " is now available.\n";
        }
        else {
            cout << "Room " << roomNumber << " is already available.\n";
        }
    }

    void displayInfo() const {
        cout << "| " << setw(10) << left << roomNumber
            << "| " << setw(15) << left << roomType
            << "| $" << setw(9) << left << pricePerNight
            << "| " << setw(10) << left << (isBooked ? "Booked" : "Available")
            << " |" << endl;
    }

    void saveToFile(ofstream& outFile) const {
        outFile << roomNumber << " " << roomType << " " << pricePerNight << " " << isBooked << endl;
    }
    //
    friend ifstream& operator>>(ifstream& inFile, Room& room) {
        inFile >> room.roomNumber >> room.roomType >> room.pricePerNight >> room.isBooked;
        return inFile;
    }
};

