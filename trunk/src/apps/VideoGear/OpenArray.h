#pragma once

#define ARRAYOF(type,...)	COpenArray<type>(__VA_ARGS__), COpenArrayCount<type>(__VA_ARGS__)

template<typename T>
class COpenArray
{
public:
	COpenArray(T arg1)
	{
		m_args = new T[1];
		m_args[0] = arg1;
	}
	COpenArray(T arg1, T arg2)
	{
		m_args = new T[2];
		m_args[0] = arg1;
		m_args[1] = arg2;
	}
	COpenArray(T arg1, T arg2, T arg3)
	{
		m_args = new T[3];
		m_args[0] = arg1;
		m_args[1] = arg2;
		m_args[2] = arg3;
	}
	COpenArray(T arg1, T arg2, T arg3, T arg4)
	{
		m_args = new T[4];
		m_args[0] = arg1;
		m_args[1] = arg2;
		m_args[2] = arg3;
		m_args[3] = arg4;
	}
	COpenArray(T arg1, T arg2, T arg3, T arg4, T arg5)
	{
		m_args = new T[5];
		m_args[0] = arg1;
		m_args[1] = arg2;
		m_args[2] = arg3;
		m_args[3] = arg4;
		m_args[4] = arg5;
	}
	COpenArray(T arg1, T arg2, T arg3, T arg4, T arg5, T arg6)
	{
		m_args = new T[6];
		m_args[0] = arg1;
		m_args[1] = arg2;
		m_args[2] = arg3;
		m_args[3] = arg4;
		m_args[4] = arg5;
		m_args[5] = arg6;
	}
	COpenArray(T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7)
	{
		m_args = new T[7];
		m_args[0] = arg1;
		m_args[1] = arg2;
		m_args[2] = arg3;
		m_args[3] = arg4;
		m_args[4] = arg5;
		m_args[5] = arg6;
		m_args[6] = arg7;
	}
	COpenArray(T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8)
	{
		m_args = new T[8];
		m_args[0] = arg1;
		m_args[1] = arg2;
		m_args[2] = arg3;
		m_args[3] = arg4;
		m_args[4] = arg5;
		m_args[5] = arg6;
		m_args[6] = arg7;
		m_args[7] = arg8;
	}
	~COpenArray(void)
	{
		delete[] m_args;
		m_args = NULL;
	}
	operator T*(void)
	{
		return m_args;
	}
private:
	T* m_args;
};

template<typename T>
class COpenArrayCount
{
public:
	COpenArrayCount(T arg1)
	{
		m_count = 1;
	}
	COpenArrayCount(T arg1, T arg2)
	{
		m_count = 2;
	}
	COpenArrayCount(T arg1, T arg2, T arg3)
	{
		m_count = 3;
	}
	COpenArrayCount(T arg1, T arg2, T arg3, T arg4)
	{
		m_count = 4;
	}
	COpenArrayCount(T arg1, T arg2, T arg3, T arg4, T arg5)
	{
		m_count = 5;
	}
	COpenArrayCount(T arg1, T arg2, T arg3, T arg4, T arg5, T arg6)
	{
		m_count = 6;
	}
	COpenArrayCount(T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7)
	{
		m_count = 7;
	}
	COpenArrayCount(T arg1, T arg2, T arg3, T arg4, T arg5, T arg6, T arg7, T arg8)
	{
		m_count = 8;
	}
	operator size_t(void)
	{
		return m_count;
	}
private:
	size_t m_count;
};