#ifndef MEMCELL_H
#define MEMCELL_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <set>
#include "Utility.h"

namespace ca
{
	class MemCell : public Object
	{
		private:
		std::string id[2];
		bool opt[3];
		MemCell* link;
		static std::set<std::string> ids;

		std::string toString() const override 
		{
			size_t n = size();
			std::stringstream out;

			if(n == 0) {return "nil";}
			while(n > 0)
			{
				n -= 1;
				out << (get(n))?("1"):("0");
			}
			return out.str();
		}
		
		bool member(const MemCell& obj) const 
		{
			if(this == &obj) {return true;}
			if(valid()) {return link->member(obj);}
			return false;
		}	
		
		static bool find(const std::string& obj) 
		{
			return !obj.empty() && ids.find(obj) != ids.end();
		}
		
		MemCell(const MemCell&) = delete;
		MemCell& operator=(const MemCell& rhs) {return *this;}

		virtual size_t length() const = 0;
		virtual bool read(size_t) const = 0;
		virtual void write(size_t,bool) = 0;
				
		protected:
		void extendable(bool ch) {opt[0] = ch;}
		void attachable(bool ch) {opt[1] = ch;}
		bool valid() const {return link != nullptr && find(id[1]);}

		const MemCell& node() const
		{
			if(valid()) {return *link;}
			return *this;
		}
 
		MemCell& node()  
		{
			if(valid()) {return *link;}
			return *this;
		}

		void bind(MemCell& obj) 
		{
			id[1] = obj.id[0];
			link = &obj;
		}

		void copy(const MemCell& obj)
		{
			link = obj.link;
			id[1] = obj.id[1];
		}

		MemCell(bool ext,bool att) : MemCell() 
		{
			extendable(ext);
			attachable(att);
		}
		
		public:
		MemCell() : opt{true,true,false}, id{"",""}, link(nullptr) 
		{
			std::stringstream out;
			out << this;
			id[0] = out.str();
			ids.insert(id[0]);
		}

		virtual ~MemCell() 
		{
			ids.erase(id[0]);
			
			if(extended()) {link->opt[2] = false;}
			link = nullptr;
		}

		size_t size() const
		{
			size_t n = length();

			if(!extended()) {return n;}
			return (n + link->size());
		}

		bool get(size_t idx) const
		{
			size_t n = (extended())?(link->size()):(0);
			size_t m = length() + n;

			if(idx < n) {return link->get(idx);}
			else if(idx < m) {return read(idx-n);}
			return false;
		}

		void set(size_t idx,bool bit) 
		{
			size_t n = (extended())?(link->size()):(0);
			size_t m = length() + n;

			if(idx < n) {link->set(idx,bit);}
			else if(idx < m) {write(idx-n,bit);}
		}
		
		void set(bool bit)
		{
			size_t m = length();

			if(extended()) {link->set(bit);}
			for(size_t i = 0;i < m;i += 1) {write(i,bit);}
		}

		void join(MemCell& obj) 
		{
			bool sf = !obj.member(*this);
		       	bool as = !appended();
			bool es = !extended();
			bool oa = !obj.appended() && obj.opt[1];

			if(sf && as && es && oa) 
			{
				bind(obj);
				obj.opt[2] = true;
			}
		}

		void release() 
		{
			if(extended()) 
			{
				link->opt[2] = false;
				link = nullptr;
				id[1] = "";
			}
		}

		bool extended() const {return opt[0] && valid();}

		bool appended() const {return opt[2];}

		void transfer(MemCell& obj) 
		{
			if(this != &obj)
			{
				size_t m = size(), n = obj.size(), i = 0;

				while(i < n && i < m)
				{
					obj.set(i,get(i));
					i += 1;
				}
			}
		}
		
		friend size_t value(MemCell& obj) 
		{
			size_t v = 0, n = obj.size();

			if(n > 32) {return 0;}
			for(size_t i = 0;i < n;i += 1)
			{
				if(obj.get(i)) {v = v ^ (1 << i);}
			}
			return v;
		}

		friend bool operator==(const MemCell& lhs,const MemCell& rhs)
		{
			size_t m = lhs.size(), n = rhs.size();
			size_t mx = (m > n)?(m):(n);

			for(size_t i = 0;i < mx;i += 1)
			{
				if(lhs.get(i) != rhs.get(i)) 
				{
					return false;
				}
			}
			return true;
		}
		
		friend bool operator!=(const MemCell& lhs,const MemCell& rhs)
		{
			return !(lhs == rhs);
		}
	};

	std::set<std::string> MemCell::ids;

	class Word : public MemCell 
	{
		private:
		std::string val;

		size_t length() const override {return val.size();}

		bool read(size_t idx) const override 
		{
			size_t n = length();

			if(idx >= n) {return false;}
			return (val[n-1-idx] == '1');
		}

		void write(size_t idx,bool bit) override 
		{
			size_t n = length();

			if(idx < n) 
			{
				val[n-1-idx] = (bit)?('1'):('0');
			}
		}

		public:
		Word() : Word(8) {}
		
		Word(size_t sz) : val("") 
		{
			if(sz == 0) {sz = 8;}
			val = std::string(sz,'0');
		}

		Word(const std::string& obj) : val(obj)
		{
			for(auto ch : obj) 
			{
				if(ch != '1' && ch != '0')
				{
					val = "0";
					break;
				}
			}
		}

		Word(const Word& obj) {*this = obj;}

		Word& operator=(const Word& rhs) 
		{
			if(this != &rhs) {val = rhs.val;}
			return *this;
		}

		virtual ~Word() {}

		friend Word operator+(const Word& lhs,const Word& rhs)
		{
			std::stringstream out;
			out << lhs << rhs;
			return Word(out.str());
		}
	};

	class Part : public MemCell 
	{	
		private:
		size_t bd[2];
		
		size_t length() const override 
		{
			return (!valid())?(0):(bd[1]-bd[0]+1);
		}

		bool read(size_t idx) const override 
		{
			size_t n = length();
			const MemCell* p = &node();

			if(this == p || idx >= n) {return false;}
			return p->get(bd[0]+idx);
		}

		void write(size_t idx,bool bit) override
		{
			size_t n = length();
			MemCell* p = &node();

			if(this != p || idx < n) {p->set(bd[0]+idx,bit);}
		}

		public:
		Part() : bd{0,0} {extendable(false);}

		Part(MemCell& obj) : Part() 
		{
			bind(obj);
			bd[1] = obj.size() - 1;
		}

		Part(MemCell& obj,size_t x,size_t y) : Part(obj)
		{
			size_t mn = (x < y)?(x):(y);
			size_t mx = (x > y)?(x):(y);
			
			if(mx <= bd[1]) 
			{
				bd[0] = mn;
				bd[1] = mx;
			}
		}

		Part(Part& obj) : Part() {*this = obj;}

		Part& operator=(Part& rhs) 
		{
			if(rhs.valid() && !this->valid())
			{
				copy(rhs);
				bd[0] = rhs.bd[0];
				bd[1] = rhs.bd[1];
			}
			return *this;
		}

		virtual ~Part() {}

		size_t begin() const {return bd[0];}	
	
		size_t end() const {return bd[1];}		

		void range(size_t x,size_t y = 0) 
		{
			MemCell* p = &node();

			if(this != p) 
			{
				size_t n = p->size();

				if(x < n && y < n) 
				{
					bd[0] = (x < y)?(x):(y);
					bd[1] = (x > y)?(x):(y);
				}
			}
		}
	};
}

#endif
