using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace stringUtils
{
    public class stringUtils
    {
        /*************** Function to get index of character in a given string. *************/

        public int getCharIndex(string inputString, char key)
        {
            int matchIndex = -1;

            for( int i = 0; i < inputString.Length; i++ )
            {
                if( inputString[i] == key )
                {
                    matchIndex = i;
                }
            }

            return (matchIndex);
        }

        public int getCharIndex(string inputString, char key, int occurence)
        {
            int matchIndex = -1;
            int occurenceCounter = 0;

            for( int i = 0; i < inputString.Length; i++ )
            {
                if( inputString[i] == key )
                {
                    occurenceCounter++;
                }

                if( occurenceCounter == occurence )
                {
                    matchIndex = i;
                    break;
                }
            }

            return (matchIndex);
        }

        /************   Function to search for a string pattern ************/

        public string matchPattern_afterMatch(string inputString, string subString)
        {
            string substringAfterMatch = "";

            for( int i = 0; i < (inputString.Length - subString.Length + 1); i++)
            {
                if( inputString.Substring(i,subString.Length) == subString )
                {
                    substringAfterMatch = inputString.Substring(i+subString.Length,
                                          inputString.Length - subString.Length - i);
                }
            }

            return (substringAfterMatch);
        }

        /***************    Function to search for a string pattern substring before match  *******/

        public string matchPattern_beforeMatch(string inputString, string subString)
        {
            string substringBeforeMatch = "";

            for (int i = 0; i < (inputString.Length - subString.Length + 1); i++)
            {
                if (inputString.Substring(i, subString.Length) == subString)
                {
                    substringBeforeMatch = inputString.Substring(0,i);
                    //Console.WriteLine(i);
                }
            }

            return (substringBeforeMatch);
        }

        /***************    Function to search for a string pattern and return index of match  *******/

        public int matchPattern_index(string inputString, string subString)
        {
            int index = -1;

            for (int i = 0; i < (inputString.Length - subString.Length + 1); i++)
            {
                if (inputString.Substring(i, subString.Length) == subString)
                {
                    index = i;
                }
            }

            return (index);
        }

        /*  Function to return characters between start and end characters. */

        public string stringBetweenCharacters(string inputString, char start, char end)
        {
            int startIndex = -1;
            int endIndex = -1;

            for( int i = 0; i < inputString.Length; i++ )
            {
                if( inputString[i] == start )
                {
                    startIndex = i;
                    break;
                }
            }

            for (int i = 0; i < inputString.Length; i++)
            {
                if (inputString[i] == end)
                {
                    endIndex = i;
                    break;
                }
            }

            if ((startIndex == -1) || (endIndex == -1))
            {
                return ("");
            }

            //Console.WriteLine($"Start Index: {startIndex}");
            //Console.WriteLine($"End Index: {endIndex}");

            return (inputString.Substring(startIndex+1, endIndex - startIndex -1));
        }



        public string stringBetweenCharacters(string inputString, char start, char end,int occurence)
        {
            int startIndex = -1;
            int endIndex = -1;
            int occurenceCounter = 0;
            string processedString = "";

            for (int i = 0; i < occurence; i++)
            {
                for (int j = 0; j < inputString.Length; j++)
                {
                    if (inputString[j] == start)
                    {
                        startIndex = j;
                    }

                    if (inputString[j] == end)
                    {
                        endIndex = j;
                    }

                    if( ( startIndex != -1 ) && ( endIndex != -1 ) )
                    {
                        inputString = inputString.Substring(startIndex);
                        break;
                    }
                }
            }


            return (inputString);
        }
    }



}
