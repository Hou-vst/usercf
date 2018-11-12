#include <iostream>
#include<map>
#include<vector>
#include<string>
#include<set>
#include<math.h>
#include<algorithm>
// ������������

using namespace std;
string A[]= {"a","b","d"};
string B[] = { "a","c" };
string C[] = { "b","e" };
string D[] = { "d","c","e" };

//���� �û�-��Ʒ�Ķ�Ӧ��
 void Create_UserToItem_Table(map<string, set<string>>& map)
{
	 set<string> v;
	 for (int i = 0; i < sizeof(A) / sizeof(string); i++)
	 {
		 v.insert(A[i]);
	 }
	 map["A"] = v;

	 v.clear();
	 for (int i = 0; i < sizeof(B) / sizeof(string); i++)
	 {
		 v.insert(B[i]);
	 }
	 map["B"] = v;

	 v.clear();
	 for (int i = 0; i < sizeof(C) / sizeof(string); i++)
	 {
		 v.insert(C[i]);
	 }
	 map["C"] = v;

	 v.clear();
	 for (int i = 0; i < sizeof(D) / sizeof(string); i++)
	 {
		 v.insert(D[i]);
	 }
	 map["D"] = v;
}

 //�û�-��Ʒ ת�� ��Ʒ-�û� ���ű�
 void TransferTo_ItemToUser(map<string, set<string>>& ori, map<string, set<string>>& dst)
 {
	 if (!dst.empty())
	 {
		 dst.clear();
	 }

	 map<string, set<string>>::iterator iter = ori.begin();
	 while (iter != ori.end())
	 {
		 set<string>& item_set = iter->second;
		 set<string>::iterator iter_set = item_set.begin();
		 for (; iter_set != item_set.end(); iter_set++)
		 {
			 dst[*iter_set].insert(iter->first);
		 }
		 iter++;
	 }
 }

 // ���ݵ��ű���ϡ�����
 void Create_CoRated_table(map<string, set<string>>& dst,map<string,map<string,int>>& CoRated_table)
 {
	 //����������Ʒ
	 map<string, set<string>>::iterator iter_begin = dst.begin();
	 map<string, set<string>>::iterator iter_End = dst.end();
	 for (; iter_begin != iter_End; iter_begin++)
	 {
		 //�������������û���
		 set<string>& user_set = iter_begin->second;
		 set<string>::iterator set_begin1 = user_set.begin();
		 for (; set_begin1 != user_set.end(); set_begin1++)
		 {
			 set<string>::iterator set_begin2 = user_set.begin();
			 string user1 = *set_begin1;
			 for (; set_begin2 != user_set.end(); set_begin2++)
			 {
				 string user2 = *set_begin2;
				 if (user1 == user2)
				 {
					 CoRated_table[user1][user2] = 0;
					 continue;
				 }
				 bool init = false;
				 map<string, map<string, int>>::iterator aim1 = CoRated_table.find(user1);
				 if (aim1 != CoRated_table.end())
				 {
					 map<string, int>& temp = aim1->second;
					 map<string, int>::iterator aim2 = temp.find(user2);
					 if (aim2 != temp.end())
					 {
						 init = true;
					 }
				 }
				
				 if(!init)
				 {
					 CoRated_table[user1][user2] = 1;
				 }
				 else
				 {
					 CoRated_table[user1][user2]++;
				 }
			 }
		 }
	 }
 }

 // �����û����û�֮������ƶ�
 void Calculate_Similarity(const map<string, map<string, int>>& CoRated_table, map<string, set<string>>& user_to_item,map<string, map<string, float>>& result)
 {
	 map<string, map<string, int>>::const_iterator iter_corated1 = CoRated_table.begin();
	 for (; iter_corated1 != CoRated_table.end(); iter_corated1++)
	 {
		 const string& user1 = iter_corated1->first;
		 const map<string, int>& table = iter_corated1->second;
		 map<string, int>::const_iterator iter_corated2 = table.begin();
		 int size1 = user_to_item[user1].size();
		 for (; iter_corated2 != table.end(); iter_corated2++)
		 {
			 const string& user2 = iter_corated2->first;
			 const int score = iter_corated2->second;
			 int size2 = user_to_item[user2].size();

			 if (user1 == user2)
			 {
				 result[user1][user2] = 0;
			 }
			 else
			 {
				 result[user1][user2] = (float)score / sqrt(size1*size2);
			 }
		 }
	 }
 }

 void PrintResult(const map<string, map<string, float>>& result)
 {
	 map<string, map<string, float>>::const_iterator iter = result.begin();
	 for (; iter != result.end(); iter++)
	 {
		 const string user1 = iter->first;
		 const map<string, float>& score = iter->second;
		 map<string, float>::const_iterator iter2 = score.begin();
		 for (; iter2 != score.end(); iter2++)
		 {
			 cout << "W[" << user1 << "][" << iter2->first << "]" << " : " << iter2->second << endl;
		 }
	 }
 }

 // ָ��һ���û�����Ʒ����������ʹ�ø���Ʒ���û��͸��û������ƶ�
 vector<float> SortSimilarUser(const map<string, map<string, float>>& table,const string& user_name,const string& item_name, map<string, set<string>>& item_to_user)
 {
	 vector<float> v;

	 if (table.find(user_name) == table.end())
	 {
		 return v;
	 }
	 const map<string, float>& user_map = table.find(user_name)->second;
	 map<string, float>::const_iterator iter = user_map.begin();
	 const set<string>& user_set = item_to_user[item_name];
	 for (; iter != user_map.end(); iter++)
	 {
		 const string& user_name1 = iter->first;

		 // �ж�user_name1�û���û��ʹ�ù�����Ʒ
		 if (user_set.find(user_name1) != user_set.end())
		 {
			 v.push_back(iter->second);
		 }
	 }

	 sort(v.begin(),v.end(),greater<float>());
	 return v;
 }
 
 float Calculate(const vector<float>& result,int k)
 {
	 float t = 0;
	 float r = 1;
	 for (int i = 0; i < result.size() && i < k; i++)
	 {
		 t += result[i]*r;
	 }

	 return t;
 }

 int main()
 {
	 // ���� �û�-��Ʒ�Ķ�Ӧ��
	 map<string, set<string>> user_to_item;
	 Create_UserToItem_Table(user_to_item);

	 // �û�-��Ʒ ת�� ��Ʒ-�û� ���ű�
	 map<string, set<string>> item_to_user;
	 TransferTo_ItemToUser(user_to_item, item_to_user);

	 // ���ݵ��ű���ϡ�����
	 map<string, map<string, int>> CoRated_table;
	 Create_CoRated_table(item_to_user, CoRated_table);

	 // �����û����û�֮������ƶ�
	 map<string, map<string, float>> result;
	 Calculate_Similarity(CoRated_table, user_to_item, result);

	 // ������
	 PrintResult(result);

	
	 string user = "B";
	 string item = "d";
	 int k = 3;
	 cout << "�����û�user :"<< user.c_str()<<"����Ʒitem :"<< item.c_str()<<"�ĸ���Ȥ�̶�,kֵΪ"<<k << endl;
	 vector<float>&& v = SortSimilarUser(result,user,item, item_to_user);
	 cout << Calculate(v, k) << endl;

	 return 0;
 }
