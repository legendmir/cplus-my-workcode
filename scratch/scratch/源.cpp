#include"tree.h"
#include<vector>
#include<windows.h>

using namespace std;

class obj
{
public:
	static int num;
};

int obj::num;




int main()
{
	vector<obj> tt;
	
	obj aa;
	aa.num = 1;
	tt.push_back(aa);

	obj aa1;
	aa1.num = 2;
	tt.push_back(aa1);

	obj aa2;
	aa2.num = 3;
	tt.push_back(aa2);

	return 0;
}