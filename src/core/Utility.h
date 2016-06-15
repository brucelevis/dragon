#ifndef Utility_h__
#define Utility_h__

#include <set>
#include <vector>
#include <algorithm>
#include "StringUtil.h"

namespace FIGHTING
{
	template <typename Set, typename Vector>
	inline void set_to_vector(Set const& theSet, Vector& array)
	{
		array.resize(0);
		array.reserve(theSet.size());
		for (typename Set::const_iterator it = theSet.begin(); it != theSet.end(); ++it)
		{
			array.push_back(*it);
		}
	}

	template <typename Map, typename Vector>
	inline void map_to_vector(Map const& theMap, Vector& array)
	{
		array.resize(0);
		array.reserve(theMap.size());
		for (typename Map::const_iterator it = theMap.begin(); it != theMap.end(); ++it)
		{
			array.push_back(it->second);
		}
	}

	template <typename T>
	class Sortor
	{
	public:
		typedef typename std::set<T> Datas;
		typedef typename Datas::iterator DatasIt;
		typedef typename Datas::const_iterator CDatasIt;

	public:
		Sortor(unsigned int max = 10) :
			max_(max)
		{

		}

		~Sortor()
		{

		}

	public:
		inline const T* GetFirst() const
		{
			if (!this->datas_.empty())
			{
				return &(*(this->datas_.begin()));
			}
			return NULL;
		}

		inline void Clear()
		{
			this->datas_.clear();
		}

		inline void Insert(T const& val)
		{
			DatasIt it = std::find(this->datas_.begin(), this->datas_.end(), val);
			if (this->datas_.end() != it)
			{
				this->datas_.erase(it);
			}
			this->datas_.insert(val);
			if (!this->datas_.empty() && (this->datas_.size() > this->max_))
			{
				DatasIt i = this->datas_.end();
				--i;
				this->datas_.erase(i);
			}
		}

		inline const unsigned int GetData(T* mem, unsigned int num) const
		{
			unsigned int ret = 0;
			CDatasIt i = this->datas_.begin();
			CDatasIt ie = this->datas_.end();
			for (; (i != ie) && (ret < num); ++i,++ret)
			{
				const T& v = *i;
				mem[ret] = v;
			}
			return ret;
		}

		inline const unsigned int GetData(std::vector<T>& mem, unsigned int num) const
		{
			unsigned int ret = 0;
			mem.clear();
			CDatasIt i = this->datas_.begin();
			CDatasIt ie = this->datas_.end();
			for (; (i != ie) && (ret < num); ++i,++ret)
			{
				const T& v = *i;
				mem.push_back(v);
			}
			return ret;
		}

		inline const unsigned int GetPlace(T const& val) const
		{
			unsigned int ret = 1;
			CDatasIt i = this->datas_.begin();
			CDatasIt ie = this->datas_.end();
			for (; i != ie; ++i,++ret)
			{
				const T& v = *i;
				if (v == val)
				{
					return ret;
				}
			}
			return 0;
		}

		inline const T* Find(T const& val) const
		{
			CDatasIt i = this->datas_.begin();
			CDatasIt ie = this->datas_.end();
			for (; i != ie; ++i)
			{
				const T& v = *i;
				if (v == val)
				{
					return &v;
				}
			}
			return NULL;
		}

		inline void Erase(T const& val)
		{
			DatasIt i = std::find(this->datas_.begin(), this->datas_.end(), val);
			if (this->datas_.end() != i)
			{
				this->datas_.erase(i);
			}
		}

		inline const unsigned int Size() const
		{
			return this->datas_.size();
		}

		inline void Dump(std::vector<T>& ret) const
		{
			set_to_vector(this->datas_, ret);
		}

	public:
		inline const unsigned int getMax() const
		{
			return this->max_;
		}

		inline void setMax(unsigned int val)
		{
			this->max_ = val;
		}

	private:
		unsigned int max_;
		Datas datas_;
	};

}

#endif // Utility_h__








