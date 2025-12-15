#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Bus.h"
#include "Memory.h"

using namespace ca;

void setBus(Bus& bus, size_t cmd, size_t addr, size_t data_val = 0)
{
    Part control(bus, bus.ctrl(), bus.ctrl() + 2);
    Part address(bus, bus.addr(), bus.addr() + 11);
    Part data(bus, bus.data(), bus.data() + 39);


    // Now we write the values i.e. We create a way to write an integer to a Part
    // Note that MemCell doesn't have a "setValue(int), so we have to write bit by bit"

    // A lambda function to write bits
    auto writeBits= [](Part& p, size_t val)
    {
        for (size_t i = 0; i < p.size(); i++)
        {
            bool bit = (val >> i) & 1;
            p.set(i, bit);
        }
    };

    writeBits(control, cmd);
    writeBits(address, addr);
    writeBits(data, data_val);
}


int main() 
{
    // We'll instantiate and link
    Bus bus;
    Memory mem;
    mem.link(bus);

    // Open the output file
    std::ofstream out("trace.txt");

    out << "--- PHASE 1: WRITES ---\n";
    // Full Write (Op 1) to Address 10
    // We'll generate random data (making sure it fits in 40bits)
    size_t full_data = rand();
    setBus(bus, 1, 10, full_data);
    mem.process();
    out << "Full Write to 10:\nBus: " << bus << "\nMem: " << mem << "\n\n";

    // Left Write (Op 3) to Address 20
    size_t left_data = rand();
    setBus(bus, 3, 20, left_data);
    mem.process();
    out << "Left Write to 20:\nBus: " << bus << "\nMem: " << mem << "\n\n";

    // Right Write (Op 5) to Address 30
    size_t right_data = rand();
    setBus(bus, 5, 30, right_data);
    mem.process();
    out << "Right Write to 30:\nBus: " << bus << "\nMem: " << mem << "\n\n";

    out << "--- PHASE 2: READS (Addr 10) ---\n";

    // Full Read (Op 0)
    // Clear bus data first (setBus defaults data to 0 if not provided)
    setBus(bus, 0, 10, 0);
    mem.process();
    out << "Full Read:\nBus: " << bus << "\n\n";

    // Left Read (Op 2)
    setBus(bus, 2, 10, 0);
    mem.process();
    out << "Left Read:\nBus: " << bus << "\n\n";

    // Right Read (Op 4)
    setBus(bus, 4, 10, 0);
    mem.process();
    out << "Right Read:\nBus: " << bus << "\n\n";

    out.close();
    return 0;
}