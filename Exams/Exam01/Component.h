#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>
#include "MemCell.h"

namespace ca
{
	class BusCom : public MemCell 
	{
		private:
		BusCom& operator=(const BusCom&) = delete;
		BusCom(const BusCom& obj) = delete;
	       
		public:
		BusCom() : MemCell(false,false) {}
		virtual ~BusCom() {}
		virtual size_t data() const = 0;
		virtual size_t addr() const = 0;
		virtual size_t ctrl() const = 0;
	};

	class Executable
	{
		private:
		BusCom* ptr;
		Executable(const Executable&) = delete;
		Executable& operator=(const Executable&) = delete;

		protected:
		BusCom& lines() 
		{
			if(ptr != nullptr) {return *ptr;}
			throw std::runtime_error("bus missing");
		}

		const BusCom& lines() const
		{
			if(ptr != nullptr) {return *ptr;}
			throw std::runtime_error("bus missing");
		}

		bool linked() const {return ptr != nullptr;}

		bool contains(const BusCom& obj) const
		{
			return linked() && ptr == &obj;
		}

		public:
		Executable() : ptr(nullptr) {}
		virtual ~Executable() {ptr = nullptr;}
		virtual void link(BusCom& obj) {ptr = &obj;}
		virtual void unlink() {ptr = nullptr;}
		virtual bool configured() const {return linked();}

		virtual void process() = 0;
		virtual std::string manual() const = 0;
	};

	class MemoryCom : public Executable, public Object
	{
		private:
		Word* locs;
		size_t lns[2];

		std::string toString() const override 
		{
			std::stringstream out;
			size_t len = 0;

			if(lns[1] > 0) {len = (size_t)ceil(log10(lns[1]));}

			for(size_t i = 0;i < lns[1];i += 1)
			{
				out << std::setw(len) << std::setfill('0');
			        out << i << ": ";
				out << locs[i] << "\n";
			}
			return out.str();
		}

		protected:
		Word& at(size_t idx) 
		{
			if(idx < lns[1]) {return locs[idx];}
			throw std::out_of_range("out of bound");
		}

		const Word& at(size_t idx) const 
		{
			if(idx < lns[1]) {return locs[idx];}
			throw std::out_of_range("out of bound");
		}

		public:
		MemoryCom() : lns{0,0}, locs(nullptr) {}

		MemoryCom(size_t sz) : MemoryCom(sz,32) {}

		MemoryCom(size_t sz,size_t bt) : MemoryCom() 
		{
			if(sz != 0)
			{
				lns[0] = (bt == 0)?(32):(bt);
				locs = new Word[sz];
				lns[1] = sz;
				for(size_t i = 0;i < lns[1];i += 1)
				{
					locs[i] = Word(lns[0]);
				}
			}
		}

		MemoryCom(const MemoryCom& obj) : MemoryCom() {*this = obj;}

		MemoryCom& operator=(const MemoryCom& rhs) 
		{
			if(this != &rhs && rhs.locs != nullptr)
			{
				if(locs != nullptr) 
				{
					delete[] locs;
					lns[0] = rhs.lns[0];
					lns[1] = rhs.lns[1];
					locs = new Word[lns[1]];
					for(size_t i = 0;i < lns[1];i += 1)
					{
						locs[i] = rhs.locs[i];
					}
				}
			}
			return *this;
		}	

		virtual ~MemoryCom() {delete[] locs;}

		size_t locations() const {return lns[1];}

		size_t bits() const 
		{
			return (locs == nullptr)?(0):(lns[0]);
		}

		void link(BusCom& obj) override
		{
			if(!contains(obj))
			{
				double l = obj.addr() - obj.ctrl();

				if(lns[1] > 0 && l <= ceil(log2(lns[1])))
				{
					Executable::link(obj);
				}
			}
		}

		bool configured() const override
		{
			return linked() && lns[3] > 0;
		}
	};
}

#endif
