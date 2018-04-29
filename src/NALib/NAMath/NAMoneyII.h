
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAMoney.h
// Do not include this file directly! It will automatically be included when
// including "NAMoney.h"



extern int na_currency_decimals[NA_CURRENCY_COUNT];

struct NAMoney{
  int64 cents;
  NACurrency currency;
};



NA_IDEF NAMoney naMakeMoneyWithCurrency(double amount, NACurrency currency){
  NAMoney money;
  double centamount = naRound(naPow(10., na_currency_decimals[currency]) * amount);
  #ifndef NDEBUG
    if(currency >= NA_CURRENCY_COUNT)
      naError("naMakeMoneyWithCurrency", "invalid currency");
  #endif
  money.cents = naMakeInt64WithDouble(centamount);
  money.currency = currency;
  return money;
}



NA_IDEF NAMoney naNegMoney(NAMoney money){
  NAMoney retmoney;
  retmoney.cents = naNegInt64(money.cents);
  retmoney.currency = money.currency;
  return retmoney;
}



NA_IDEF NAMoney naAddMoney(NAMoney moneyA, NAMoney moneyB){
  NAMoney money;
  #ifndef NDEBUG
    if(moneyA.currency != moneyB.currency)
      naError("naAddMoney", "currencies do not match");
  #endif
  money.cents = naAddInt64(moneyA.cents, moneyB.cents);
  money.currency = moneyA.currency;
  return money;
}



NA_IDEF NAMoney naSubMoney(NAMoney moneyA, NAMoney moneyB){
  NAMoney money;
  #ifndef NDEBUG
    if(moneyA.currency != moneyB.currency)
      naError("naSubMoney", "currencies do not match");
  #endif
  money.cents = naSubInt64(moneyA.cents, moneyB.cents);
  money.currency = moneyA.currency;
  return money;
}



NA_IDEF NAString* naNewStringWithMoney(NAMoney money){
  int digits = na_currency_decimals[money.currency];
  int64 divisor = naMakeInt64WithDouble(pow(10., (double)digits));
  int64 units = naDivInt64(money.cents, divisor);
  int64 decimals = naModInt64(money.cents, divisor);
  if(digits == 0){
    return naNewStringWithFormat("%lld", units);
  }else{
    NAString* formatstring = naNewStringWithFormat("%%lld.%%0%dlld", (int)digits);
    NAString* retstring = naNewStringWithFormat(naGetStringUTF8Pointer(formatstring), units, decimals);
    naDelete(formatstring);
    return retstring;
  }
}


// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
