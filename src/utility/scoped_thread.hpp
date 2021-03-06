/* 
 * Copyright (c) 2016 - 2017 cooky451
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#pragma once

#include <thread>

namespace utility // export
{
	template <typename DestructionPolicy>
	class ScopedThread
	{
		std::thread _thread;

	public:
		~ScopedThread()
		{
			DestructionPolicy()(_thread);
		}

		ScopedThread(ScopedThread&& other)
			: _thread(std::move(other._thread))
		{}

		ScopedThread& operator = (ScopedThread&& other)
		{
			_thread = std::move(other._thread);
			return *this;
		}

		ScopedThread(std::thread&& thr = std::thread())
			: _thread(std::move(thr))
		{}

		auto& get()
		{
			return _thread;
		}

		auto& get() const
		{
			return _thread;
		}

		auto release()
		{
			return std::move(_thread);
		}
	};

	struct ScopedThreadAutoJoinPolicy
	{
		void operator () (std::thread& thr) const
		{
			if (thr.joinable())
			{
				thr.join();
			}
		}
	};

	struct ScopedThreadAutoDetachPolicy
	{
		void operator () (std::thread& thr) const
		{
			if (thr.joinable())
			{
				thr.detach();
			}
		}
	};

	using AutojoinThread = ScopedThread<ScopedThreadAutoJoinPolicy>;
	using AutodetachThread = ScopedThread<ScopedThreadAutoDetachPolicy>;
}
