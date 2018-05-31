#pragma once

#include <atomic>

namespace core {
	
	template< typename T, size_t size >
	class LockFreeCircularBufferSPSC {
	public:		
		LockFreeCircularBufferSPSC()
			: _head(0)
			, _tail(0)
		{}

		~LockFreeCircularBufferSPSC()
		{}
	
		bool IsEmpty() const {
			return _head.load() == _tail.load();
		}
		
		bool IsFull() const {
			const auto nextTail = Increment( _tail.load() );
			return nextTail == _head.load();
		}
	
		bool IsLockFree() const {
			return true; //vishal: later
		}

		bool Push( const T& element ) {
			const auto currentTail = _tail.load();
			const auto nextTail = Increment(currentTail);
			if( nextTail != _head.load() ) {
				_array[currentTail] = element;
				_tail.store(nextTail);
				return true;
			}
			else 
				return false;
		}

		bool Pop( T& element ) {
			const auto currentHead = _head.load();
			if( currentHead != _tail.load() ) {
				element = _array[currentHead];
				_head.store( Increment(currentHead) );
				return true;
			}
			else
				return false;
		}

	private:
		size_t Increment( size_t index ) const {
			return (index+1) % _capacity;
		}

	private:
		std::atomic<size_t> _head;
		T _array[size];
		std::atomic<size_t> _tail;
		const size_t _capacity = size + 1;
	};
}
