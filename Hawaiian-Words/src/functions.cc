#include "functions.hpp"

#include <iostream>

/**
 * @param word word to change to lowercase
 * @return result of changing every character in word to lowercase
 */
std::string StringToLower(const std::string& word) {
  std::cout << "Input to StringToLower() : " << word << std::endl;
  std::string output;
  for (auto ch : word) {
    output.push_back(std::tolower(ch));
  }
  return output;
}

/**
 * @param v1 first character in potential vowel group
 * @param v2 second character in potential vowel group
 * @return true if v1 and v2 form a valid vowel group
 */
bool IsVowelGroup(const char& v1, const char& v2) {
  std::cout << "Input to IsVowelGroup() : \"" << v1 << "\" \"" << v2 << "\""
            << std::endl;
  if (!IsValidCharacter(v1) || !IsValidCharacter(v2)) return false;

  // Hawaiian language vowel group rules
  if (v1 == 'a' && (v2 == 'i' || v2 == 'e' || v2 == 'o' || v2 == 'u'))
    return true;
  if (v1 == 'e' && (v2 == 'i' || v2 == 'u')) return true;
  if (v1 == 'i' && v2 == 'u') return true;
  if (v1 == 'o' && (v2 == 'i' || v2 == 'u')) return true;
  if (v1 == 'u' && v2 == 'i') return true;

  if (v1 == 'i' && v2 == 'a') return false;

  return false;
}

/**
 * @param v potential vowel character
 * @return true if v is a vowel
 */
bool IsVowel(const char& v) {
  std::cout << "Input to IsVowel() : \"" << v << "\"" << std::endl;
  return (v == 'a' || v == 'e' || v == 'i' || v == 'o' || v == 'u');
}

/**
 * @param c potential consonant character
 * @return true if c is a consonant
 */
bool IsConsonant(const char& c) {
  std::cout << "Input to IsConsonant() : \"" << c << "\"" << std::endl;
  return (c == 'p' || c == 'k' || c == 'h' || c == 'l' || c == 'm' ||
          c == 'n' || c == 'w');
}

/**
 * @param c character to check validity of
 * @return true if input c is a valid character in the Hawaiian language, false
 * otherwise
 */
bool IsValidCharacter(const char& c) {
  std::cout << "Input to IsValidCharacter() : \"" << c << "\"" << std::endl;
  return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
          c == 'p' || c == 'k' || c == 'h' || c == 'l' || c == 'm' ||
          c == 'n' || c == 'w' || c == ' ' || c == '\'');
}

/**
 * @param word word to check validity of
 * @return true if every character in word is a valid character in the Hawaiian
 * language, false otherwise
 */
bool IsValidWord(const std::string& word) {
  std::cout << "Input to IsValidWord() : " << word << std::endl;
  for (auto ch : word) {
    if (!IsValidCharacter(ch)) return false;
  }
  return true;
}

/**
 * @param c consonant to get pronunciation of
 * @param prev character before c, used for determining w translation, set to
 * null character as default if no value is passed in
 * @return pronunciation of the consonant c as a char
 */
char ConsonantPronunciation(const char& c, const char& prev) {
  std::cout << "Input to ConsonantPronunciation() : \"" << c << "\" \"" << prev
            << "\"" << std::endl;

  if (c == 'p') return 'p';
  if (c == 'k') return 'k';
  if (c == 'h') return 'h';
  if (c == 'l') return 'l';
  if (c == 'm') return 'm';
  if (c == 'n') return 'n';

  if (c == 'w') {
    if (prev == 'u' || prev == 'o' || prev == 'a' || prev == ' ') return 'w';
    if (prev == 'i' || prev == 'e')
      return 'v';  // Fix to return 'v' after 'i' or 'e'
    return 'w';    // Default case to handle if previous character doesn't match
                   // the vowel conditions
  }

  return '\0';
}

/**
 * @param v1 first vowel in a vowel grouping
 * @param v2 second vowel in a vowel grouping
 * @return the pronunciation of the vowel grouping made up of v1 and v2 as a
 * string
 */
std::string VowelGroupPronunciation(const char& v1, const char& v2) {
  std::cout << "Input to VowelGroupPronunciation() : \"" << v1 << "\" \"" << v2
            << "\"" << std::endl;
  if (v1 == 'a') {
    return (v2 == 'i' || v2 == 'e')   ? "eye"
           : (v2 == 'o' || v2 == 'u') ? "ow"
                                      : "";
  }
  if (v1 == 'e') {
    return (v2 == 'i') ? "ay" : (v2 == 'u') ? "eh-oo" : "";
  }
  if (v1 == 'i' && v2 == 'u') return "ew";
  if (v1 == 'o') {
    return (v2 == 'i') ? "oy" : (v2 == 'u') ? "ow" : "";
  }
  if (v1 == 'u' && v2 == 'i') return "ooey";
  return "";
}

/**
 * @param v single vowel to get pronunciation of
 * @return the pronunciation of v as a string
 */
std::string SingleVowelPronunciation(const char& v) {
  std::cout << "Input to SingleVowelPronunciation() : \"" << v << "\""
            << std::endl;
  if (v == 'a') return "ah";
  if (v == 'e') return "eh";
  if (v == 'i') return "ee";
  if (v == 'o') return "oh";
  if (v == 'u') return "oo";
  return "";
}

/**
 * @param prev first character in set of three passed to function
 * @param curr second character in set of three passed to function
 * @param next third character in set of three passed to function
 * @return pronunciation of curr using next and prev as needed to determine
 * result
 */
std::string ProcessCharacter(const char& prev,
                             const char& curr,
                             const char& next) {
  std::cout << "Input to ProcessCharacter() : \"" << prev << "\" \"" << curr
            << "\" \"" << next << "\"" << std::endl;

  std::string result;

  if (IsConsonant(curr)) {
    if (curr == 'w') {
      result += ConsonantPronunciation(curr, prev);
    } else {
      result += ConsonantPronunciation(curr);
    }
  }

  else if (IsVowel(curr)) {
    if (IsVowelGroup(curr, next)) {
      result += VowelGroupPronunciation(curr, next);
    } else {
      result += SingleVowelPronunciation(curr);
    }
  }

  else {
    result += curr;
  }

  return result;
}

std::string Pronunciation(const std::string& word) {
  std::cout << "Input to Pronunciation() : " << word << std::endl;
  std::string pronunciation;

  for (std::size_t i = 0; i < word.size(); ++i) {
    if (word[i] == ' ') {
      pronunciation += " ";  // Preserve spaces in the pronunciation
      continue;
    }

    if (i == 0) {
      pronunciation += ProcessCharacter('\0', word[i], word[i + 1]);
      if (IsVowelGroup(word[i], word[i + 1])) {
        i++;  // Skip next character if part of a vowel group
      } else if (IsVowel(word[i]) && word[i + 1] != ' ' &&
                 word[i + 1] != '\'' && i < word.size() - 1) {
        pronunciation += '-';  // Add hyphen after a single vowel
      }
      continue;
    }

    pronunciation += ProcessCharacter(word[i - 1], word[i], word[i + 1]);

    if (IsVowelGroup(word[i], word[i + 1])) {
      i++;  // Skip next character if part of a vowel group
      if (i < word.size() - 1 && word[i + 1] != ' ' && word[i + 1] != '\'') {
        pronunciation += '-';  // Add hyphen after vowel group
      }
    } else if (IsVowel(word[i]) && word[i + 1] != ' ' && word[i + 1] != '\'' &&
               i < word.size() - 1) {
      pronunciation += '-';  // Add hyphen after a single vowel
    }
  }
  return pronunciation;
}

/**
 * @param hawaiian_word word to get pronunciation of
 * @return the pronunciation of hawaiian_word as a string if valid and throws an
 * invalid_argument error if not valid
 */
std::string GetPronunciation(const std::string& hawaiian_word) {
  std::cout << "Input to GetPronunciation() : " << hawaiian_word << std::endl;
  std::string lower_word = StringToLower(hawaiian_word);
  if (!IsValidWord(lower_word)) {
    throw std::invalid_argument("Invalid Argument");
  }
  return Pronunciation(lower_word);
}
