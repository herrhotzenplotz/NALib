#include "../../NALib/src/NALib.h"

// This is already included by including NALib.h but here you can find all
// relevant APIs:
#include "../../NALib/src/NAUtility/NAJSON.h"

typedef struct TinyObject TinyObject;
struct TinyObject {
  NAUTF8Char* message;
};

typedef struct SimpleValues SimpleValues;
struct SimpleValues {
  int64 member1;
  float member2;
  double member3;
  int32 member4;
  void* member5;
  NAUTF8Char* member6;
  TinyObject object1;
  TinyObject* object2;
};

typedef struct ArrayValues ArrayValues;
struct ArrayValues {
  int32 values1[5];
  size_t count2;
  int32* values2;
  NAUTF8Char* values3[5];
  size_t count4;
  NAUTF8Char** values4;
};

typedef struct Test Test;
struct Test {
  SimpleValues simpleValues;
  ArrayValues arrayValues;
};



NAJSONParser* allocateSimpleParser(void){
  NAJSONParser* parser = naAllocateJSONParser();

  NAJSONRuleSet* tinyRules1 = naRegisterJSONRuleSet(parser);
  naAddJSONRule(tinyRules1, "message", naNewJSONRuleString(offsetof(Test, simpleValues.object1.message)));

  NAJSONRuleSet* tinyRules2 = naRegisterJSONRuleSet(parser);
  naAddJSONRule(tinyRules2, "message", naNewJSONRuleString(offsetof(TinyObject, message)));

  NAJSONRuleSet* simpleRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(simpleRules, "value1", naNewJSONRuleInt64(offsetof(Test, simpleValues.member1)));
  naAddJSONRule(simpleRules, "value2", naNewJSONRuleFloat(offsetof(Test, simpleValues.member2)));
  naAddJSONRule(simpleRules, "value3", naNewJSONRuleDouble(offsetof(Test, simpleValues.member3)));
  naAddJSONRule(simpleRules, "value4", naNewJSONRuleInt32(offsetof(Test, simpleValues.member4)));
  naAddJSONRule(simpleRules, "value5", naNewJSONRulePointerObject(
    offsetof(Test, simpleValues.member5),
    0,
    NA_NULL));
  naAddJSONRule(simpleRules, "value6", naNewJSONRuleString(offsetof(Test, simpleValues.member6)));
  naAddJSONRule(simpleRules, "value7", naNewJSONRuleObject(tinyRules1));
  naAddJSONRule(simpleRules, "value8", naNewJSONRulePointerObject(
    offsetof(Test, simpleValues.object2),
    sizeof(TinyObject),
    tinyRules2));

  NAJSONRuleSet* arrayRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(arrayRules, "values1", naNewJSONRuleFixedArrayInt32(
    offsetof(Test, arrayValues.values1),
    5));
  naAddJSONRule(arrayRules, "values2", naNewJSONRuleArrayInt32(
    offsetof(Test, arrayValues.values2),
    offsetof(Test, arrayValues.count2)));
  naAddJSONRule(arrayRules, "values3", naNewJSONRuleFixedArrayString(
    offsetof(Test, arrayValues.values3),
    5));
  naAddJSONRule(arrayRules, "values4", naNewJSONRuleArrayString(
    offsetof(Test, arrayValues.values4),
    offsetof(Test, arrayValues.count4)));

  NAJSONRuleSet* baseRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(baseRules, "SimpleObject", naNewJSONRuleObject(simpleRules));
  naAddJSONRule(baseRules, "ArraysObject", naNewJSONRuleObject(arrayRules));

  // The whole file contains one (unnamed) object.
  NAJSONRuleSet* fileRules = naRegisterJSONRuleSet(parser);
  naAddJSONRule(fileRules, "", naNewJSONRuleObject(baseRules));

  naSetJSONParserInitialRules(parser, fileRules);
  return parser;
}



int jsonExample(void){

  naStartRuntime();
  
  // Loading the buffer from a file.
  NADateTime now1 = naMakeDateTimeNow();
  NAFile* file = naCreateFileReadingPath("res/JSONinput.txt");
  NAFileSize bufferSize = naComputeFileByteSize(file);
  NAByte* buf = naMalloc(bufferSize + 1);
  naReadFileBytes(file, buf, bufferSize);
  // Buffer needs to be closed with a \0 byte.
  buf[bufferSize] = '\0';
  NADateTime now2 = naMakeDateTimeNow();

  printf("Time: %f milliseconds to read file\n", 1000. * naGetDateTimeDifference(&now2, &now1));

  // Creating a parser which reads into the Test structure.
  Test test;
  NAJSONParser* simpleParser = allocateSimpleParser();

  // Running a benchmark
  #define TESTCOUNT 100
  NADateTime now3 = naMakeDateTimeNow();
  for(int i = 0; i < TESTCOUNT; ++i){
    // Parse the buffer into the test variable.
    naParseJSONBuffer(simpleParser, &test, buf, bufferSize + 1);
  }
  NADateTime now4 = naMakeDateTimeNow();

  printf("Time: %f milliseconds to parse file\n", 1000. * naGetDateTimeDifference(&now4, &now3) / (double)TESTCOUNT);
  printf("Variable \'test\' now contains all desired values.\n\n");

  // Deallocating the parser also deallocates all rules and ruleSets.
  naDeallocateJSONParser(simpleParser);

  naFree(buf);

  naStopRuntime();

  return 0;
}
