#include <iostream>
//#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <cstdio>
#include <fstream>

using namespace std;
map <string,float> mp; // for storing bound variables
stack <char> opr;      // operator stack
stack <string> ex;     // expression stack

class PsilEvaluation
{
	public:
	
	PsilEvaluation(){ }
	/* function for evaluating expression whene operator occurs */
	void OperatorEvaluation(vector <float> v,char op){
		if (op == '+') {
			float sum = 0;
			for(int i = 0;i < v.size();i++)sum += v[i];
			string srm = NumtoString(sum);
			ex.push(srm);
		} else if (op == '*') {
			float mul = 1;
			for(int i = 0;i < v.size();i++)mul *= v[i];
			string srm = NumtoString(mul);
			ex.push(srm);
		 }else if(op == '-'){
			float TotalSub = 0;
			for(int i = 0;i < v.size()-1;i++) TotalSub += v[i];
				TotalSub = v[v.size()-1]-TotalSub;
			string srm = NumtoString(TotalSub);
			ex.push(srm);
		}
		else{
			float TotalDiv = 1.00;
			bool flag = false;
			for(int i = 0;i < v.size()-1;i++)TotalDiv *= v[i];

			if(TotalDiv == 0.00 || TotalDiv == 0) { ex.push("error");return;}
			else{
				TotalDiv = v[v.size()-1]/TotalDiv;
				string srm = NumtoString(TotalDiv);
				ex.push(srm);
			}
		}
	}
	/* function for converting string to num */
	float StringToNum(string str)
	{
		float num = 0;
		int dig = 0;
		int neg = 0;
		if (mp[str]) {
			return mp[str];
		} else if (isdigit(str)) {		
			for(int i = 0 ;i < str.size();i++)
			{
				if(str[i] == '.') { dig = 1;continue; }
				if(str[0] == '-')neg = 1;
				num  = num*10 +  (str[i] -'0');
				dig = dig*10;

			}
			if(dig)num = num/dig;
			if(neg)num = (-1)*num;
		} else {
			cout << "Invalid program\n";return -1; 
		}
		return num;

	}
	/* function for checking a string is number or not*/
	bool isdigit(string str) {
		int i = 0;
		if(str[i] == '-')i++;
		while (i < str.size() && ((str[i] >= '0' && str[i] <= '9') || str[i] == '.'))i++;
		if (i == str.size())
			return true;
		else if (mp[str]) {
			return true;
		}
		else {	
			return false;
		}
	}
	/*Function for extracting next element in expresison */
	string ExtractNext(string &str,int &spos) {
		string ans = "";
		while(spos < str.size() && str[spos] == ' ')spos++;

		if(str[spos] == '(' || str[spos] == '+' || str[spos] == '*' || str[spos] == '-') return string(1,str[spos]);

		while(spos < str.size() && str[spos] != ' ' && str[spos] != ')' && str[spos] != '('){ans+=str[spos];spos++;}
		return ans;
	}
	/* Function for converting number to string */
	string NumtoString(float n)
	{
		stringstream ss ;
		ss << n;
		string str;
		ss >> str;
		return str;

	}
	/* function for storing expression information in map , stack  */ 
	string EvaluateExpression(string str)
	{

			for (int i = 0; i < str.size(); i++) {
				//cout << "index" << i << " " << str[i] << endl;
				if (str[i] == '(') {
					int j = i+1;
					string chk = ExtractNext(str,j);
					if(chk != "+" && chk != "-" && chk != "*" && chk != "/" && chk != "bind" && chk != "(") return "Invalid program";
					ex.push("(");	
				} else if ( str[i] == ')') {
					if(opr.empty()) { return "Invalid program";}
					char op = opr.top();opr.pop();
					if (op == '=') {
						string v1 = ex.top();ex.pop();
						while(isdigit(ex.top()) && !mp[ex.top()])ex.pop();
						if (ex.top() == "(") { 
							ex.pop();
							ex.push(v1);
							opr.push(op);
							continue;				
						}	
						string v2 = ex.top();ex.pop();
						//cout << v1 << " " << v2 << endl;
						if (isdigit(v1)) {
							mp[v2] = StringToNum(v1);
						} else {
							return "Invalid program";
						}
						if (ex.top() == "(") { 
							ex.pop();
							ex.push(NumtoString(StringToNum(v1)));
						}	
						else{
							return "Invalid program";
						}	
					} else {
						vector <float> temp;
						while(!ex.empty() && ex.top() != "(" )
						{
							temp.push_back(StringToNum(ex.top()));
							ex.pop();	
						}
						ex.pop();
						if(temp.size() == 1){ return "Invalid program";}
						else OperatorEvaluation(temp, op); // sending for evaluation
					}		
				} else if (str[i] == '+') {
					opr.push('+');
				} else if (str[i] == '*') {				
					opr.push('*');
				}else if (str[i] == '-') {				
					opr.push('-');
				}
				else if (str[i] == '/') {				
					opr.push('/');
				}
				 else if (str[i] == ' ') {
					continue;
				} else { 
					string no = ExtractNext(str, i);
					//cout << "next thing->" << no << endl;
					if (no == "bind") {
						string var = ExtractNext(str, i);
						opr.push('=');
						ex.push(var);
					} else if ( isdigit(no)) {
						ex.push(no);
					} else {
						//continue;				
						return "Invalid program";
					}
					i--;
				}

		}
		if (opr.empty() && !ex.empty() && ex.top() != "error") {
			return ex.top();
		} else if(!opr.empty()) return "Invalid program";
		else {
			return "Invalid program";
		}
	}
};
int main()
{

	PsilEvaluation Obj;

	ifstream infile;
	ofstream outfile;
	infile.open("input.txt");  // reading file
	outfile.open("output.txt"); // for writing output
	string str,srm;
	vector <string> v;
	while(getline(infile,str))
	{	
		if(str == "")    // seperating by new empty line  //
		{	//cout << "empty\n";
			if(v.size())
			{
	 			string OneLineInput = "";
				for (int i = 0; i < v.size(); ++i)OneLineInput += v[i];
				outfile << Obj.EvaluateExpression(OneLineInput) << endl;outfile << endl;  // 
				v.clear();mp.clear();
	 			ex = stack<string>();
	 			opr = stack<char>();
	 		}
	 		else continue;
		}
		else
		{	
				stringstream ss;	
				string s1;
				srm =  "";
				ss << str;
				while(ss >> s1)	{ srm += s1;srm += " ";}
				v.push_back(srm);
				
			}
	}
	//for last input ,in file //
	string lastInput = "";
	for (int i = 0; i < v.size(); ++i){ lastInput += v[i];}
	outfile << Obj.EvaluateExpression(lastInput) << endl;
	
	infile.close();
	outfile.close();
	return 0;

	

}
