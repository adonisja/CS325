#ifndef BUS_H
#define BUS_H

#include "Component.h"

namespace ca {
    class Bus : public BusCom  //inherits from BusCom
    {
        private:
            ca::Word storage;   // Holds all the bits of data


            /* These are the important helpers used to access information on the system bus
            These define the WHAT (3bits), WHERE (12bits) and the WHO (remainder/40bits) */
            ca::Part control_lines;    // section holding the bits with control data (e.g. WRITE, READ, etc.)
            ca::Part address_lines;  // section holding bits with the address data (where in memory to look for the data. e.g. 000000000101 = 5 in binary, so look in slot #5)
            ca::Part data_lines;    // section containing the actual data content (data will be copied from this section to the memory slot in a WRITE operation or from the memory slot to these bits in a READ operation)

        public:
            Bus() :
            // We initialize the storage with the total number of bits (3 Ctrl, 12 Addr, 40 Data so 55 total)
            storage(55),

            /* Then we map each part to a storage location; convention dictates
            Control: [0-2], Address: [3-14], Data [15-54]
            */
            control_lines(storage, 0, 2),
            address_lines(storage, 3, 14),
            data_lines(storage, 15, 54)
            {}

            // Then we create the Mandatory overrides from MemCell
            size_t length() const override { return storage.size(); } // Tells us how big is the whole bus

            bool read(size_t idx) const override {return storage.get(idx); } // Allows us to read a specific location/index

            void write(size_t idx, bool bit) override { storage.set(idx, bit); } // Writes data to a specific location/index

            // We also create the overrides for the BusCom methods
            size_t ctrl() const override { return 0; } // Tells the Memory that the Control Lines start at index 0
            size_t addr() const override { return 3; } // Tells the Memory that the Address Lines start at index 3
            size_t data() const override { return 15; }  // Tells the Memory that the Data Lines start at index 15
    };
}
#endif