#pragma once
#include <cstdlib>

class new_string
{
public:

	new_string() {}

	template < typename T>
	new_string(T string)
	{
		assign_w_string(string);
	}

	/* copy constructor uses contents  of old wchar*, as the old string's wchar* will be freed when it leaves scope */

	new_string(const new_string& constructor_string)
	{
		assign_w_string(constructor_string.wide_string);
		string_size = constructor_string.string_size;
	}

	void operator=(new_string string)
	{
		if (string.wide_string)
			assign_w_string(string.w_str());
	}

	template < typename T>
	void operator=(T string)
	{
		if (string)
			assign_w_string(string);
	}

	void operator+=(new_string additional_string)
	{
		if (!additional_string.wide_string)
			return;

		wchar_t* new_string = (wchar_t*)malloc((string_size + additional_string.string_size) * sizeof(wchar_t));

		for (int i = 0; i < string_size; i++)
			new_string[i] = wide_string[i];

		for (int i = 0; i <= additional_string.string_size; i++)
			new_string[string_size - 1 + i] = additional_string.wide_string[i];

		free(wide_string);

		/* we only kept one of the trailing '\0' from the strings, so new size is size of both strings - 1 */
		string_size = string_size + additional_string.string_size - 1;
		wide_string = new_string;
		make_c_string();
	}

	new_string operator+(new_string additional_string)
	{
		/* is the string we were just passed a nullptr? if so just return ourselves */
		if (!additional_string.wide_string)
			return *this;

		wchar_t* tmp_string = (wchar_t*)malloc((string_size + additional_string.string_size) * sizeof(wchar_t));

		for (int i = 0; i < string_size; i++)
			tmp_string[i] = wide_string[i];

		for (int i = 0; i <= additional_string.string_size; i++)
			tmp_string[string_size - 1 + i] = additional_string.wide_string[i];

		new_string new_obj = tmp_string;
		free(tmp_string);
		return new_obj;
	}

	wchar_t operator[] (int index)
	{
		return (index < string_size - 1) ? wide_string[index] : throw std::out_of_range("Invalid iterator");
	}

	template < typename T>
	bool operator==(T compare_against)
	{
		return equal(compare_against);
	}

	template < typename T>
	bool operator!=(T compare_against)
	{
		return !equal(compare_against);
	}

	/* free our internal wchar_t* on destruction */
	~new_string()
	{
		if (wide_string)
			free(wide_string);
		if (c_string)
			free(c_string);
	}

	/* getter functions */

	wchar_t* w_str()
	{
		return wide_string;
	}

	char* c_str()
	{
		return c_string;
	}

	size_t length()
	{
		/* the size we use internally is that of the full string, but
		the user doesn't care about the terminating '\0' */
		return string_size - 1;
	}

	template < typename T>
	void append(T string)
	{
		*this += string;
	}

	template < typename T>
	bool equal(T string)
	{
		new_string compare_string = new_string(string);
		if (!compare_string.wide_string)
			return false;
		if (compare_string.string_size != string_size)
			return false;
		for (int i = 0; i < string_size; i++)
			if (wide_string[i] != compare_string.wide_string[i])
				return false;

		return true;
	}

	template < typename T>
	bool contains(T string)
	{
		new_string compare_string = new_string(string);
		if (!compare_string.wide_string)
			return false;
		if (compare_string.string_size > string_size)
			return false;

		for (int i = 0; i < string_size - 1; i++)
		{
			if (wide_string[i] == compare_string.wide_string[0])
			{
				for (int j = 0; j < compare_string.string_size - 1; j++)
				{
					if (wide_string[i + j] != compare_string.wide_string[j])
						break;

					if (j == (compare_string.string_size - 2))
						return true;
				}
			}
		}

		return false;
	}

private:

	template < typename T>
	void assign_w_string(T string)
	{
		if (!string)
			return;

		if (wide_string)
			free(wide_string);

		/* add +2 to the legnth, as we start indexing at zero and our legnth is that of the full string, which
		includes the '\0' */
		for (int i = 0; string[i] != '\0'; i++)
			string_size = i + 2;

		wide_string = (wchar_t*)malloc(string_size * sizeof(wchar_t));

		for (int i = 0; i <= string_size - 1; i++)
			wide_string[i] = string[i];

		make_c_string();
	}

	void make_c_string()
	{
		if (!wide_string)
			return;

		if (c_string)
			free(c_string);

		/* add +2 to the legnth, as we start indexing at zero and our legnth is that of the full string, which
		includes the '\0' */

		c_string = (char*)malloc(string_size * sizeof(char));

		for (int i = 0; i <= string_size - 1; i++)
		{
			if ((int)wide_string[i] <= 127)
				c_string[i] = wide_string[i];
			else
				c_string[i] = '?';
		}
	}

	wchar_t* wide_string = 0;
	char*	 c_string	 = 0;

	size_t	 string_size = 0;
};
