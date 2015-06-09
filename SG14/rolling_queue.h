#include <array>

namespace sg14
{
	template<typename T, std::size_t capacity>
	class rolling_queue
	{
	public:
		typedef std::array<T, capacity> Container;
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef typename Container::iterator iterator;
		typedef typename Container::const_iterator const_iterator;
		typedef typename Container::reverse_iterator reverse_iterator;
		typedef typename Container::const_reverse_iterator const_reverse_iterator;

		rolling_queue() noexcept(std::is_nothrow_default_constructible<T>::value)
			: c()
			, count(0)
			, next_element(std::begin(c))
			, last_element(next_element)
		{
		}

		rolling_queue(const rolling_queue& rhs)
			: c(rhs.c)
			, count(rhs.count)
			, next_element(std::begin(c) + (rhs.next_element - std::begin(rhs.c)))
			, last_element(std::begin(c) + (rhs.last_element - std::begin(rhs.c)))
		{
		}

		rolling_queue(rolling_queue&& rhs) noexcept(std::is_nothrow_move_constructible<T>::value)
			: c(std::move(rhs.c))
			, count(std::move(rhs.count))
			, next_element(std::begin(c) + (rhs.next_element - std::begin(rhs.c)))
			, last_element(std::begin(c) + (rhs.last_element - std::begin(rhs.c)))
		{
		}

		rolling_queue& operator=(const rolling_queue& rhs)
		{
			c = rhs.c;
			return (*this);
		}

		rolling_queue& operator=(rolling_queue&& rhs) noexcept(std::is_nothrow_move_assignable<T>::value)
		{
			c = std::move(rhs.c);
			return (*this);
		}

		bool push(const value_type& from_value)
		{
			if (count == capacity)
			{
				return false;
			}
			*next_element = from_value;
			increase_back();
			return true;
		}

		bool push(value_type&& from_value)
		{
			if (count == capacity)
			{
				return false;
			}
			*next_element = std::move(from_value);
			increase_back();
			return true;
		}

		template<class... FromType>
		bool emplace(FromType&&... from_value)
		{
			if (count == capacity)
			{
				return false;
			}
			*next_element = T(std::forward<FromType>(from_value)...);
			increase_back();
			return true;
		}

		void pop()
		{
			*last_element = T();
			increase_front();
		}

		bool empty() const noexcept
		{
			return (next_element == last_element);
		}

		size_type size() const noexcept
		{
			return count;
		}

		reference front()
		{
			return (*last_element);
		}

		const_reference front() const
		{
			return (*last_element);
		}

		reference back()
		{
			auto it = decrease(next_element);
			return (*it);
		}

		const_reference back() const
		{
			auto it = decrease(next_element);
			return (*it);
		}

		void swap(rolling_queue& rhs) noexcept
		{
			auto lhs_next = next_element - c.begin();
			auto lhs_last = last_element - c.begin();
			auto rhs_next = rhs.next_element - rhs.c.begin();
			auto rhs_last = rhs.last_element - rhs.c.begin();
			std::swap(c, rhs.c);
			std::swap(count, rhs.count);
			next_element = c.begin() + rhs_next;
			last_element = c.begin() + rhs_last;
			rhs.next_element = rhs.c.begin() + lhs_next;
			rhs.last_element = rhs.c.begin() + lhs_last;
		}

	private:
		Container c;
		size_t count;
		iterator next_element;
		iterator last_element;

		void increase_back()
		{
			if (++next_element == c.end())
			{
				next_element = c.begin();
			}
			++count;
		}

		iterator decrease(iterator it)
		{
			if (it == c.begin())
			{
				it = c.end();
			}
			return --it;
		}

		void increase_front()
		{
			if (++last_element == c.end())
			{
				last_element = c.begin();
			}
			--count;
		}
	};
}