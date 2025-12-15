#ifndef MEMORY_H
#define MEMORY_H

#include "Component.h"

namespace ca {
    class Memory : public MemoryCom
    {
        public:
            // We'll build a constructor with 4096 locations, defining 40 bits per word
            Memory () : MemoryCom(4096, 40) {}

            void process() override
            {
                // We'll get the bus
                if (!configured()) return; // Safety check

                BusCom& bus = lines();  // Get which bus we are linked to

                /* Now I need to create Parts for each Bus component to
                move the data around. */

                // The Control is 3 bits wide (0, 1, 2)
                Part control(bus, bus.ctrl(),bus.ctrl() + 2);

                // The Address is 12 bits wide (3-15) 
                Part address(bus, bus.addr(), bus.addr() + 11);

                // Data is 40 bits wide (15-54)
                Part data(bus, bus.data(), bus.data()+ 39);

                /* A word of caution, the above represent relative offsets and
                 not absolute indices from Bus.h; for the address Part we overrode the bus.addr() method
                 to return 3 so Part address(bus, bus.add(), bus.addr() + 11) says
                 call the address lines, start on index 3, end on 3+11 = 14 (these are inclusive indices) */

                size_t cmd = value(control);        // decodes the command and converts the bits to integers (e.g. 001 -> 1). This tells us what we will do
                size_t addr_val = value(address);  // decodes the address and converts the bits to integers; this tells us where we'll execute the command (memory slot #)

                Word& mem_word = at(addr_val);  // Then we grab the actual data from memory (the target Word from storage)

                /* We'll then create Parts for the Memory Word and the Bus Data Sub-sections (In essence Empty "Part" containers to "hold" different views of the Word)
                Remember the Bus Data is 40 bits wide, we'll split it [20bits left | 20 bits right]
                */
                Part mem_left(mem_word, 0, 19); // Create a Part object for the left section of the Memory Word
                Part mem_right(mem_word, 20, 39);  // Create a Part object for the right section of the Memory Word
                Part bus_left(data, 0, 19);  // Create a Part object for the left section of the Bus Data
                // We use Part objects to abstract the object creation for each section (We can just assign to the Part container instead of running a loop to assign each bit for each section which would make it tedious af...)

                // Now we connect each Window/Part" based on the command
                switch (cmd)
                {
                    case 0:     // 000 - Full Read (Memory -> Bus)
                        mem_word.transfer(data);
                        break;
                    case 1:   // 001 - Full Write (Bus -> Memory)
                        data.transfer(mem_word);
                        break;
                    case 2:     // 010 - Left Read (Memory Left -> Bus Left)
                        mem_left.transfer(bus_left);
                        break;
                    case 3:     // 011 - Left Write (Bus Left -> Memory Left)
                        bus_left.transfer(mem_left);
                        break;
                    case 4:     // 100 - Right Read (Memory Right -> Bus Left)
                        mem_right.transfer(bus_left);
                        break;
                    case 5:     // 101 - Right Write (Bus Left -> Memory Right)
                        bus_left.transfer(mem_right);
                        break;
                }
            }

            std::string manual() const override
            {
                return  "000: Full Read\n"
                        "001: Full Write\n"
                        "010: Left Read\n"
                        "011: Left Write\n"
                        "100: Right Read\n"
                        "101: Right Write\n";
            }
    };
}

#endif