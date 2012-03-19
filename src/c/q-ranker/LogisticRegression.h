/*******************************************************************************
 Copyright (c) 2008 Lukas K�ll

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software. 
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
 $Id: LogisticRegression.h,v 1.5 2008/06/10 21:35:14 lukall Exp $
 
 *******************************************************************************/

#ifndef LOGISTICREGRESSION_H_
#define LOGISTICREGRESSION_H_
#include<vector>
using namespace std;
#include "BaseSpline.h"

namespace qranker {

class LogisticRegression : public BaseSpline
{
public:
  LogisticRegression();
  virtual ~LogisticRegression();
  void predict(const vector<double>& x, vector<double>& predict) { return BaseSpline::predict(x,predict);}
  void setData(const vector<double>& xx, const vector<unsigned int>& yy, const vector<unsigned int>& mm) {
  	  BaseSpline::setData(xx);y=yy; m=mm;}
protected:
  virtual void calcPZW();
  virtual void initg();
  vector<unsigned int> y,m;
  Vec p;
};

} // qranker namespace

#endif /*LOGISTICREGRESSION_H_*/