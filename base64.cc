#include <iostream>
#include <string>
#include <cassert>

using namespace std;


string convertToBase64(const string& binaryInput) 
{
  const unsigned int sourceWidth = 8;
  const unsigned int targetWidth = 6;

  assert( targetWidth <= sourceWidth );

  int nextInputIndex = 0;
  string output;

  const unsigned int totalBits = binaryInput.size() * sourceWidth; 
  unsigned int bitsLeft = totalBits;

  unsigned char buffer = 0;
  unsigned char remainder = 0;
  unsigned short remainderBits = 0;

  const unsigned short offset = 65; // start ASCII value of printable character set to use
  
  if (bitsLeft == 0)
  {
    return output;
  }

  for (;;)  
  {
    if (bitsLeft <= targetWidth)
    { // handle last (incomplete) input byte
      // Last part is now in remainder
      // we have at least one input byte, and asserted that targetWidth is smaller than sourceWidth, so this case cannot happen on first for loop iteration
      //cout << "last character: remainder is " << remainder << " and has " << remainderBits << " bits" << endl;
      assert( bitsLeft == remainderBits );
      output += (remainder + offset);      
      break;
    }
    else
    {
      // two cases: either construct buffer from next input byte and remainder, if remainderBits < targetBits
      //            or construct buffer enitrely from remainder if remainderBits >= targetBits
      if (remainderBits >= targetWidth)
      {
        buffer = remainder;
        buffer = buffer >> (remainderBits - targetWidth);
        output += (buffer + offset);
        // new remainder is low-order unused remainder bits
        remainder = remainder & (((unsigned char) ~0u) >> (sourceWidth - remainderBits + targetWidth));
        remainderBits -= targetWidth;
     
        //cout << "Reusing remainder as it has " << remainderBits << " bits" << endl;
      }
      else 
      { // Must get bits from next input byte
        //cout << "Constructing buffer as remainder has only " << remainderBits << " bits" << endl;
	buffer = (unsigned char) binaryInput[nextInputIndex]; 
        // extract high (targetWidth - remainderBits)  bits from buffer
        buffer = buffer >> (sourceWidth - (targetWidth - remainderBits));
        // Add remainder
        buffer = buffer | (remainder << (targetWidth - remainderBits));
        output += (buffer + offset);
        remainder = (unsigned char) binaryInput[nextInputIndex]; 
        // Save unused bits of current input byte
        remainder = remainder & (((unsigned char) ~0u) >> (targetWidth - remainderBits)) ;
        remainderBits = sourceWidth - (targetWidth - remainderBits);
        ++nextInputIndex; // current input byte content has been saved in buffer and remainder
      }
 
      bitsLeft -= targetWidth;
    }
  }
  return output;
}


int main(int argc, char** argv) {
  char zeroes = 0;
  char ones =   ~0;

  string test;
  //test += ones;
  //test += zeroes;
  test += 'U';
  test += 'U';
  test += 'U'; // character '0' has ASCII code 48 = binary 00110000; U is 85
  cout << convertToBase64(test) << endl;
  return 0;
}
