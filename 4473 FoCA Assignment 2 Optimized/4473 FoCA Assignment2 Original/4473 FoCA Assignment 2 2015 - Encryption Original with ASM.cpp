// The encryption program in C++ and ASM with a very simple encryption method - it simply adds 2 then bitshifts the encription key by
// 2 and xor it with the character.
// The encryption method is written in ASM. You will replace this with your allocated version for the assignment.
// In this version parameters are passed via registers (see 'encrypt' for details).
// Filename: "4473 FoCA 2015 Encryption Original with ASM.cpp"
//
// liam hill, last update: Feb 2015
//  *** PLEASE CHANGE THE LINE ABOVE TO YOUR NAME & DATE ***

#include <conio.h>		// for kbhit
#include <iostream>		// for cin >> and cout <<
#include <iomanip>		// for fancy output
using namespace std;

#define MAXCHARS 6		// feel free to alter this, but 6 is the minimum
#define dollarchar '$'  // string terminator

char OChars[MAXCHARS],
	 EChars[MAXCHARS],
	 DChars[MAXCHARS];	// Global Original, Encrypted, Decrypted character strings

//----------------------------- C++ Functions ----------------------------------------------------------

void get_char (char& a_character)
{
	cin >> a_character;
	while (((a_character < '0') | (a_character > 'z')) && (a_character != dollarchar))
	{	cout << "Alphanumeric characters only, please try again > ";
		cin >> a_character;
	}
}
//-------------------------------------------------------------------------------------------------------------

void get_original_chars (int& length)
{	char next_char;
	length = 0;
	get_char (next_char);
	while ((length < MAXCHARS) && (next_char != dollarchar))
	{
		OChars [length++] = next_char;
		get_char (next_char);
	}
}

//---------------------------------------------------------------------------------------------------------------
//----------------- ENCRYPTION ROUTINES -------------------------------------------------------------------------
// edi = i
// ebx = adress to ecar[i] 
// ecx = char
// eax = ekey
void encrypt_chars (int length, char EKey)
{   
		__asm {									//
			xor		edx, edx					// edx = i = 0
		loop1:									// top of the loop (while loop)
			cmp	   edx, [ebp+8] 				// compare i - length
			je	   endofloop					// check if i is outside bounds of ochars
			lea	   ecx, [edx*1+OChars]			// get the location of char in ochars
			mov    ecx, [ecx]					// get the value of a char in the ochars
			lea	   eax, EKey					// get the adress of ekey
			add	   [eax], 0x02;					// ekey + 2
			ror	   byte ptr[eax], 2;			// bitshift ekey >> 2
			xor	   ecx, [eax];					// combine with encription key
			ror	   cl, 1;						// char = char * 2
			lea	   ebx, [edx*1+EChars]			// get where to place the new char in echars
			mov	   [ebx], ecx					// send encripted char to echars	
			inc    edx							// incroment EDX
			jmp	   loop1						// back to the top
		endofloop:								// exit marker of the loop
	}
	//--- End of Assembly code
}
// end of encrypt_chars function
//---------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------
//----------------- DECRYPTION ROUTINES -------------------------------------------------------------------------
//
void decrypt_chars (int length, char EKey)
{
	__asm {
			 ret 99
				xor		edx, edx				// edx = i = 0
		loop1 :								// top of the loop (while loop)
			cmp	   edx, [ebp + 8] 			// compare i - length
			je	   endofloop				// check if i is outside bounds of echars
			lea	   eax, [edx * 1 + EChars]	// get the location of char in ochars
			mov    eax, [eax]				// get the value of a char in the echars
			lea	   ecx, EKey				// get the adress of ekey
			rol	   al, 1;					// 8 bit register of ecx
			add	   [ecx], 0x02;				// 
			ror	   byte ptr[ecx], 2;		// bitshift >> 2 
			xor	   eax, [ecx];				// revert the diffrence
			and	   eax, 0xFF;				// bad tolower
			push   eax						// not needed in relece mode 
			call   tolower					// convert upercase  to lowercase
			mov    eax, [esp-12]			// get result
			add    esp, 4					// remove some
			lea	   ebx, [edx * 1 + DChars]	// get where to place the new char in dchars
			mov	   [ebx], eax				// send encripted char to echars	
			inc    edx						// incroment EDX
			jmp	   loop1					// back to the top
		endofloop :							// exit marker of the loop
	}
   // decription function char decript11(char* ekey, char temp_char)
}
// end of decrypt_chars function
//---------------------------------------------------------------------------------------------------------------
int main(void)
{
	int  char_count;	// The number of actual characters entered (upto MAXCHARS limit).
	char EKey;			// Encryption key c.
		cout << sizeof(char) << endl;
		cout << "\nPlease enter your Encryption Key (EKey) letter: ";
		get_char (EKey);
		
		cout << "\nNow enter upto " << MAXCHARS << " alphanumeric characters:\n";
		get_original_chars (char_count);
		cout << "\n\nOriginal source string = " << OChars << "\tHex = ";
		for (int i=0; i<char_count; i++)
			cout<<hex<<setw(2)<<setfill('0')<< ((int (OChars[i]))&0xFF)<<"  ";//display

		encrypt_chars (char_count, EKey);
		cout << "\n\nEncrypted string       = " << EChars << "\tHex = ";
		for (int i=0; i<char_count; i++)
			cout<< ((int (EChars[i]))&0xFF)<<"  ";//display

		decrypt_chars (char_count, EKey);
		cout << "\n\nDecrypted string       = " << DChars << "\tHex = ";
		for (int i=0; i<char_count; i++) 
			cout<< ((int (DChars[i]))&0xFF)<<"  ";//display

	cout << "\n\nPress a key to end...";
	while ( !_kbhit());					//hold the screen until a key is pressed
	return (0);
} // end of whole encryption/decryption program --------------------------------------------------------------------