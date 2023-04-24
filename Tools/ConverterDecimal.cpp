#include "ConverterDecimal.h"

ConverterDecimal::ConverterDecimal()
{
}


ConverterDecimal::~ConverterDecimal()
{
}

inline long ConverterDecimal::ToBinary(int number)
{
	int count = 0;
	int resulting_array[25];
	int size = 0;

	long binary_number = 0;
	long base = 1;

	while (number > 0)
	{
		int qutiont = number / 2;
		int remainder = number % 2;
		number = qutiont;
		resulting_array[count] = remainder;

		size++;
		count++;
	}

	for (int j = 0; j < size; j++)
	{
		long sum = resulting_array[j] * base;
		base = base * 10;
		binary_number += sum;
	}

	return binary_number;
}

std::vector< bool> ConverterDecimal::ToBinaryArray(int number, int Length)
{
	int count = 0;
	std::vector<bool> resulting_array;

	while (number > 0)
	{
		int qutiont = number / 2;
		int remainder = number % 2;
		number = qutiont;
		resulting_array.push_back(remainder);

		count++;
	}

	for (int j = count; j < Length; j++)
	{
		resulting_array.push_back(false);
	}

	return resulting_array;
}

inline double ConverterDecimal::ToHex(int number)
{
	int count = 0;
	int resulting_array[25];
	int size = 0;

	double pental_number = 0;
	double base = 1;

	while (number > 0)
	{
		int qutiont = number / 5;
		int remainder = number % 5;
		number = qutiont;
		resulting_array[count] = remainder;

		size++;
		count++;
	}

	for (int j = 0; j < size; j++)
	{
		double sum = resulting_array[j] * base;
		base = base * 10;
		pental_number += sum;
	}
	return pental_number;
}

inline double ConverterDecimal::ToOctal(int number)
{
	int count = 0;
	int resulting_array[25];
	int size = 0;

	double octal_number = 0;
	double base = 1;

	while (number > 0)
	{
		int qutiont = number / 8;
		int remainder = number % 8;
		number = qutiont;
		resulting_array[count] = remainder;

		count++;
		size++;
	}

	for (int j = 0; j < size; j++)
	{
		double sum = resulting_array[j] * base;
		base = base * 10;
		octal_number += sum;
	}
	return octal_number;
}

//************************************
// Method:    decimal_binary
// FullName:  ConverterDecimal::decimal_binary
// Access:    public 
// Returns:   int
// Qualifier: /* Function to convert decimal to binary.*/
// Parameter: int n
//************************************
inline int ConverterDecimal::Decimal_Binary(int n)  /* Function to convert decimal to binary.*/
{
	int i = 1, binary = 0;
	while (n != 0)
	{
		int rem = n % 2;
		n /= 2;
		binary += rem*i;
		i *= 10;
	}
	return binary;
}

inline int ConverterDecimal::Binary_Decimal(int n) const
/* Function to convert binary to decimal.*/
{
	int decimal = 0, i = 0;
	while (n != 0)
	{
		int rem = n % 10;
		n /= 10;
		decimal += rem * pow(2, i);
		++i;
	}
	return decimal;
}

inline int ConverterDecimal::Binary_Decimal_Array(std::vector<bool> ArrayList) const
/* Function to convert binary to decimal.*/
{
	int decimal = 0, i = 0;
	for each (bool var in ArrayList)
	{
		decimal += var * pow(2, i);
		++i;
	}
	return decimal;
}
