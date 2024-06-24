
# Header file containing methods to help string handling

#
#  @brief: Function to get the first occurence of a certain character in a string.
#

def getCharIndex(inString,keyChar):

  index = 0;
  keyIndex = -1;
  currentCharacter = ' ';

  for currentCharacter in inString:
    if( currentCharacter == keyChar):
      keyIndex = index;
    index = index + 1;
      
  return(keyIndex)
  
#
# @brief: Function to get the n th occurence of a certain character in a string.
#  

def getCharIndex(inString,keyChar,occurence):

  index = 0;
  keyIndex = -1;
  currentCharacter = ' ';
  occurenceCounter = 0;
  
  for currentCharacter in inString:
    if( currentCharacter == keyChar ):
      occurenceCounter = occurenceCounter + 1
      if( occurenceCounter == occurence ):
        keyIndex = index;
    index = index + 1
        
  return(keyIndex)

#
# @brief: Function to get the substring between two characters.
#  

def getSubstring(inString,keyChar_1,occurence_1,keyChar_2,occurence_2):

  index_1 = getCharIndex(inString,keyChar_1,occurence_1);
  index_2 = getCharIndex(inString,keyChar_2,occurence_2);
    
  return( inString[ index_1+1 : index_2 ] )
  
#
# @define: Function to get the index of pattern match.
#

def getPatternMatchIndex(inString,inPattern):
  
  counter = 0;
  patternLength = len(inPattern)
  stringLength = len(inString)
  index = -1;

  while (counter < ( stringLength - patternLength + 1 )):
    if (inString[counter:counter+patternLength] == inPattern):
        index = counter;
    counter = counter + 1;        
  
  return(index);
  
#
# @define: Function to get string after pattern match.
#  

def getStringAfterPatternMatch(inString,inPattern):
  return( inString[getPatternMatchIndex(inString,inPattern)+len(inPattern):] ) 
  
#
# @define: Function to get string after before match.
#  

def getStringBeforePatternMatch(inString,inPattern):
  return( inString[:getPatternMatchIndex(inString,inPattern)] ) 

#
# @define: Function to get string before character.
#  

def getStringBeforeChar(inString,charKey,occurence):
  index = getCharIndex(inString,charKey,occurence);
  return( inString[:index] )
  
#
# @define: Function to get string after character.
#  

def getStringAfterChar(inString,charKey,occurence):
  index = getCharIndex(inString,charKey,occurence);
  return( inString[index:] )
  
#
# @brief: Function to get the number of occurences of a certain character in a string.
#  

def getNumberOfMatches(inString,inChar):
  matches = 0;
  
  for character in inString:
    if( character == inChar ):
      matches = matches + 1;
      
  return(matches)