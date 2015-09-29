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
	 DChars[MAXCHARS] = "Soon!";	// Global Original, Encrypted, Decrypted character strings

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

void encrypt_chars (int length, char EKey)
{   
	char temp_char;						// char temporary store
	int i = 0;
	//for (i = 0; i < length; i++)	// encrypt characters one at a time
	//{
 		//temp_char = OChars [i];		//
		__asm {							//
		loop1:							// top of the loop (while loop)
			mov	   edx, i				// move i into EDX
			cmp	   edx, [ebp+8] 		// compare i - length = negative number 
			je	   endofloop			// check if i is outside bounds of array
			lea	   ecx, [edx*1+OChars]	// get the item in the array
			mov    ecx, [ecx]			// get the value to write to memory
			mov	   temp_char, cl		// put value back into temp_char 
			push   eax					// save register values on stack to be safe
			push   ecx					//
										//
			movzx  ecx, temp_char		// set up registers Cdecl
			lea    eax, EKey			// 
			push   ecx;					// converted to cdecal the right way
			push   eax;					// 
			call   encrypt11			// encrypt the character
			pop    eax					// get result
			mov    temp_char, al		// 16 bit register
			add	   esp, 4				// cleanup the 2 pramiters
										// 
			pop    ecx					// restore original register values from stack
			pop    eax					// 
			movzx  eax, temp_char		// get the encripted char back
			lea	   ecx, [edx*1+EChars]	// get where to place the new char
			mov	   [ecx], eax			// send to the array	
			inc    i					// incroment i
			jmp	   loop1				// back to the top
		endofloop:						// exit marker of the loop
	//	}
		//EChars [i] = temp_char;			// Store encrypted char in the encrypted chars array
	}
   return;


	// Encrypt subroutine. You should paste in the encryption routine you've been allocated from Bb and
	// overwrite this initial, simple, version. Ensure you change the ‘call’ above to use the
	// correct 'encryptnn' label where nn is your encryption routine number.
	// Inputs: stack    EBP+8  = 32-bit address of Ekey,
	//					EBP+12 = the character to be encrypted (in the low 8-bit field, CL).
	// Output: stack    ESP+4  = the encrypted value of the source character (in the low 8-bit field, AL).
   __asm {
   encrypt11:
	   push		ebp;						// cdecal style
	   mov		ebp, esp;					// setup the stack
	   mov		eax, [ebp+8];				// get varibles
	   mov		ecx, [ebp+12];				// 
	   push		edx;						// save nv registers
	   push		ebx;						// 
	   push		ecx;						// 
	   and		dword ptr[eax], 0x000000FF;	// remove extra bits from the value at EAX
	   add		[eax], 0x02;				// ekey + 2
	   ror		byte ptr[eax], 1;			// bitshift ekey >>2
	   ror		byte ptr[eax], 1;			// 
 	   mov		edx, [eax];					// get value of EAX (ekey)
	   pop		ebx;						// get temp_char
	   xor		ebx, edx;					// combine with encription key
	   ror		bl, 1;						// 8 bit register of EBX
	   mov		eax, ebx;					// move encripted c har to EAX
	   pop		ebx;						// restore nv registers
	   pop		edx;						// 
	   mov		esp, ebp;					// can be removed if all is ok
	   pop		ebp;						// restore other stack
	   mov		[esp+4],  eax;				// push result to stack (in the perfect place)
	   ret;									// finish
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
	char temp_char;						// char temporary store

	for (int i = 0; i < length; i++)	// encrypt characters one at a time
	{
		temp_char = EChars[i];
		__asm {							//
			movzx  eax, temp_char		// set up registers Cdecl
			lea    ecx, EKey			// 
			push   eax;					// converted to cdecal the right way
			push   ecx;					// 
			call   decript11			// decript the character
			call   tolower				// convert to lowercase
			add	   esp, 8				// cleanup 4 for decript11 4 for tolower
			mov    eax,[esp - 8]		// get result 
			mov    temp_char, al		// 16 bit register
		}
		DChars[i] = temp_char;
	}
   return;
   // decription function char decript11(char* ekey, char temp_char)
   __asm {
   decript11:
	   push		ebp;						// cdecal style
	   mov		ebp, esp;					//
	   mov		ecx, [ebp + 8];				// get varibles
	   mov		eax, [ebp + 12];			//
	   rol		al, 1;						// 8 bit register of ecx
	   and		dword ptr[ecx], 0x000000FF;	// remove extra bits
	   add		[ecx], 0x02;				// ekey + 2
	   ror		byte ptr[ecx], 2;			// bitshift ekey >> 2 
	   xor		eax, [ecx];					// revert the diffrence
	   pop		ebp;						// restore stack
	   mov		[esp + 4], eax;				// push decripted char to stack
	   ret;									// finish
   }
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


