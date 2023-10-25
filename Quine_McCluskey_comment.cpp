#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <iomanip>
#include <time.h>
#include <algorithm>
#include <set>
using namespace std;

///////////////////////////////////////////////////////////////////
//                            main.cpp                           //
///////////////////////////////////////////////////////////////////

vector<int>ON_set;
vector<int>DC_set;
vector<string>Implicant;
vector<string>Second_Implicant;
vector<bool>used;
vector<string> ON_set_binary;
vector<string> part1_answer;
vector<string> answer;
vector<int> column_sum; //debug 
int num_var;
bool merged=0;


string dec2bin(int n){
    string binary_str;
    for(int i=0 ; i < num_var ; i++){
        binary_str += n%2 +'0';
        n=n/2;
    }
     reverse(binary_str.begin(), binary_str.end());
    // cout << binary_str <<endl<<endl;
     return binary_str;
     
}

bool merge(const string a,const string b,vector<string>& Second_Implicant  ){
    string str;
    string c=a;
    int count1=0;
    int position_;
    bool merged =  false;
    bool same =0 ;
    for(int i=0 ; i < a.length() ; i++){
        if((a[i]!=b[i])){
             count1 = count1+1;
             position_ = i;
        }
    }

    if(count1==1){
        c[position_] = '-'; 
        for(int i=0 ; i<Second_Implicant.size() ; i++){ // 檢查 Second_Implicant裡面是不是有相同的01-1了
            if(c == Second_Implicant[i])
                same = 1;
        }
        if(!same){
            Second_Implicant.push_back(c);
            merged = true;
            // cout << "same  "<< endl << endl;
            // cout<<"input_a:" <<a << " input_b:" <<b <<"  output_C:"<< c << endl;
        }
    }
    //cout << merged;
    return merged;
}

bool Implicant_caculation (vector<string>& Implicant,vector<string>& Second_Implicant,vector<bool>& used){
    bool done =1;
    for(int i =0; i<Implicant.size() ; i++){
            for(int  j=i+1; j<Implicant.size();j++){
                merged = merge(Implicant[i],Implicant[j],Second_Implicant );
                if(merged){
                    //  cout <<i <<" "<<j << " "<<merged;
                    // cout   << endl << endl;
                    used[i] = 1;
                    used[j] = 1;
                    done = 0;
                }
            }
        }
    return done;
}   

bool same_funtion(const string a ,const string b){
    bool same = true;
    for(int i=0 ; i<a.length() ; i++){
        if((a[i]!=b[i])&(a[i]!='-')&(b[i]!='-'))
            same = false;
    }
    return same;
}

int num_dash(const string a){
    int num = 0;
    for(int i=0 ; i<a.length() ; i++){
        if(a[i]=='-')
            num = num+1;
    }
    return num;
}
void cal_column_sum(vector<string>&Implicant ,vector<string>& ON_set_binary  ,vector<string>& answer, vector<int>& column_sum  ) {
   bool same;
   string temp_imp;
   int sum = 0 ;
   bool answer_ext = false;
   for(int i=0 ; i<ON_set_binary.size() ; i++){
       sum = 0;
       for(int j=0 ; j<Implicant.size() ; j++){
          same = same_funtion( Implicant[j] , ON_set_binary[i]);
           if(same==1){
             sum = sum+1;
             temp_imp = Implicant[j];
            // cout << endl << endl;
            // cout   <<"i:"<<i <<"  j:"<<j<<"  Implicant:"<<  Implicant[j] <<" is same as on :"<< ON_set_binary[i]<< endl;

           }
       }
       // 算完一個column的值了
    //    cout  <<endl<<endl;
    //    cout << sum;
        answer_ext = false;
        column_sum.push_back(sum);
        if(sum==1){
            for(int i=0 ; i<answer.size();i++){
                if(temp_imp == answer[i])
                    answer_ext=1;
            }
            if(!answer_ext){
                answer.push_back(temp_imp);
            }
        }
   }
}



void remove_vector( vector<string>& ON_set_binary , string target) {
    // 要移除的特定值 target
    // 使用 remove 將特定值移到 vector 的末尾
    ON_set_binary.erase(remove(ON_set_binary.begin(), ON_set_binary.end(), target), ON_set_binary.end());
    
}

void reduced_table(vector<string>& target , vector<string>&Implicant , vector<string>& ON_set_binary  ){
    bool same = 0;
    vector<string> remove_string; //debug 
    for(int i=0 ; i<target.size() ; i++){
        for(int j=0 ; j<ON_set_binary.size() ; j++){
            same = same_funtion( target[i] , ON_set_binary[j]);
            // cout<<ON_set_binary[j]<<" " <<"same "<<same ;
            // cout << endl<< endl;
            if(same)
               remove_string.push_back(ON_set_binary[j]);
        }
        for(int i=0 ; i<remove_string.size() ; i++){
            remove_vector(ON_set_binary,remove_string[i] );
        }
        remove_string.clear();
         remove_vector(Implicant,target[i] );
    }

}


string row_sum(vector<string>&Implicant ,vector<string>& ON_set_binary  ,vector<string>& answer  ) {
   bool same;
   string temp_imp;
   int sum = 0 ;
   int temp_row = 0;
   int temp_dash_num = 0;
   int new_dash_num =0;
   for(int i=0 ; i<Implicant.size() ; i++){
       sum = 0;
       new_dash_num=0;
       for(int j=0 ; j<ON_set_binary.size() ; j++){
          same = same_funtion( Implicant[i] , ON_set_binary[j]);
           if(same==1){
             sum = sum+1;
           }
       }
       // 算完一個row的值了
        if(sum >temp_row){
            temp_imp =  Implicant[i];
            temp_row = sum;
            temp_dash_num = num_dash(Implicant[i]);
            // cout << endl << endl;
            // cout << "row  number"<<temp_row  <<"  Implicant  "<<temp_imp << "  i:"<<i<< endl;
        }
        else if(sum == temp_row){
            new_dash_num = num_dash(Implicant[i]);
            if(new_dash_num > temp_dash_num){
                temp_imp =  Implicant[i];
                temp_dash_num = new_dash_num;
            // cout << endl << endl;
            // cout << "row  number"<<temp_row  <<"  Implicant  "<<temp_imp << "  i:"<<i<< endl;
            }

        }
   }
   return temp_imp;
}

string binary_2_ABC(string answer ) {
   string answer_ABC;
   for(int i=0; i<answer.length();i++){
        if(answer[i]=='1'){
            answer_ABC += 'A'+i;
        }
        else if(answer[i]=='0'){
            answer_ABC += 'A'+i;
            answer_ABC += 39; // '  ascii is 39
        }
   }
   return answer_ABC;
}

int count_literal(vector<string>& answer ) {
    int literal = 0;
     string temp ;
     for(int i=0; i<answer.size();i++){
         temp = answer[i];
         for(int j=0; j<temp.length();j++){
              if((temp[j] =='1')||(temp[j] =='0')){
                  literal ++;
              }
         }
     }
    return literal;
}




int main(int argc, char* argv[])
{
    
    
   ifstream inFile; // 創建一個 ifstream 對象
    inFile.open(argv[1]); // 打開文件
    ofstream fout(argv[2]); //output	
    if (!inFile) {
        //cout << "無法打開文件" << endl;
        return 1;
    }
      
    string line;
    //stringstream input;
    int int_temp;
    while (getline(inFile, line)) {
        stringstream input;
        // cout<<"getline  "<<line;
        if(line == ".i"){
            getline(inFile, line);
            // cout<<".i  "<<line;
            input << line;
            input >> num_var; 
        }

        if(line == ".m"){
            getline(inFile, line);
            input << line;

            while(input >> int_temp){

                ON_set.push_back(int_temp);
            }
        }

        if(line == ".d"){
            getline(inFile, line);
            input << line;
            while(input >> int_temp){
                DC_set.push_back(int_temp);
            }
        }
    }
    // cout << "var num = " << num_var << endl << endl;
	// cout << "on set / dcset = ";
    // for(int i=0 ; i<ON_set.size() ; i++){
	// 	//cout << ON_set[i] << " ";
	// }

    // for(int i=0 ; i<DC_set.size() ; i++){
	// 	cout << DC_set[i] << " ";
	// }

    inFile.close(); // 關閉文件

    for(int i=0 ; i<ON_set.size() ; i++){
		Implicant.push_back(dec2bin(ON_set[i]));
	}
    for(int i=0 ; i<DC_set.size() ; i++){
		Implicant.push_back(dec2bin(DC_set[i]));
	}

    // sort(Implicant.begin(), Implicant.end());
    // sort(Implicant.begin(), Implicant.end(), sort_num_1);
     

    // cout   << endl << endl;
    // cout << "Implication table column 1 " << endl << endl;
    // for(int i=0 ; i<Implicant.size() ; i++){
	// 	cout << Implicant[i] << " ";
	// }
    
     ON_set_binary.resize(ON_set.size() );
     ON_set_binary.assign  (Implicant.begin(),Implicant.begin()+ON_set.size());

    // cout   << endl << endl;
    // cout << "On set binary " << endl << endl;
    // for(int i=0 ; i<ON_set_binary.size() ; i++){
	// 	cout << ON_set_binary[i] << " ";
	// }
    // string initial_Implicant;
    // initial_Implicant = Implicant;

    bool Implicant_done = false;

    while(!Implicant_done){
        // cout   << endl << endl;
        // cout << "Implicant column " << endl << endl;
        // for(int i=0 ; i<Implicant.size() ; i++){
    	//     cout << Implicant[i] << " ";
    	//  }
        used.resize(Implicant.size() ); // to recored the data is merged
        Implicant_done = Implicant_caculation(Implicant ,Second_Implicant,used);
        // cout <<"done" << Implicant_done;
         for(int i=0 ; i<used.size() ; i++){
             if(!used[i] && !Implicant_done){
                Second_Implicant.push_back(Implicant[i]); //used 為0要寫回 Second_Implicant
             }
             used[i]=0;//判斷玩used 歸零
          }
         if(!Implicant_done){
             //used.resize(Second_Implicant.size() );
             Implicant.resize(Second_Implicant.size() );
             Implicant = Second_Implicant;
             Second_Implicant.clear();
         }

            // cout   << endl << endl;
            // cout << "column Implicant " << endl << endl;
            // for(int i=0 ; i<Implicant.size() ; i++){
    		//     cout << Implicant[i] << " ";   /////////////part-1 answer
    	    // }
    }
    
   
    //
    sort(Implicant.begin(), Implicant.end()); // c++ string '-' < '0' < '1'  (ASCII)
                                               //  '-'(45) '1'(48)

    reverse(Implicant.begin(), Implicant.end()); //sort預設小到大 我們輸出規定要大到小
    // cout   << endl << endl;
    // cout << "Implication table part1 final output " << endl << endl;

    // for(int i=0 ; i<Implicant.size() ; i++){
	// 	cout << Implicant[i] << " ";   /////////////part-1 answer
	// }
    part1_answer.resize(Implicant.size());
    part1_answer = Implicant;
    // cout   << endl << endl;

 ///////////////////////////////////////////////////////////////////
//                            part 2                            //
///////////////////////////////////////////////////////////////////
 
    cal_column_sum(Implicant , ON_set_binary  , answer,column_sum );
    // cout   << endl << endl;
    // cout   << "column_sum" << endl;
    
    // for(int i=0 ; i<column_sum.size() ; i++){
	// 	cout << column_sum[i] << " ";
    //         cout   << endl << endl;
	// }

    reduced_table(answer , Implicant , ON_set_binary);
    //  cout   << endl << endl;
    // for(int i=0 ; i<answer.size() ; i++){
	// 	cout << answer[i] << " ";
    //         cout   << endl << endl;
	// }

    string row_max;
    vector<string> remove;
    int i=0;
    while(ON_set_binary.size()!=0){
        row_max =row_sum(Implicant , ON_set_binary  , answer);
        answer.push_back(row_max);
        remove.push_back(row_max);
        reduced_table(remove , Implicant , ON_set_binary);
        remove.clear();
        i= i+1;
    // cout   << endl << endl;
    // cout << "the "<<i <<"  round" <<  endl;
    // for(int i=0 ; i<answer.size() ; i++){
	// 	cout << answer[i] << " ";
    //         cout   << endl << endl;
	// }
    }

    // cout   << endl << endl;
    // cout   <<"ON set size "<< ON_set_binary.size() << endl;
    sort(answer.begin(), answer.end()); // c++ string '-' < '0' < '1'  (ASCII)
                                               //  '-'(45) '1'(48)
    reverse(answer.begin(), answer.end()); //sort預設小到大 我們輸出規定要大到小



    // cout   << endl << endl;
    // for(int i=0 ; i<ON_set_binary.size() ; i++){
	// 	cout << ON_set_binary[i] << " ";
	// }

    //  cout   << endl << endl;
    // for(int i=0 ; i<Implicant.size() ; i++){
	// 	cout << Implicant[i] << " ";
	// }



    string temp;
     vector<string> answer_ABC;
     for(int i=0 ; i<part1_answer.size() ; i++){
        answer_ABC.push_back(binary_2_ABC(part1_answer[i]));
	 }

    cout   << endl << endl;
    cout << "part1_answer" <<  endl;
    cout<<" .p"<< answer_ABC.size();
    cout   << endl << endl;
    for(int i=0 ; i<answer_ABC.size() ; i++){
		cout << answer_ABC[i] << " ";
         cout   << endl << endl;
	}

    // cout   << endl << endl;
    // cout << "final_answer" <<  endl;
    // for(int i=0 ; i<answer.size() ; i++){

	// 	cout << answer[i] << " ";

    //         cout   << endl << endl;
	// }


    vector<string> final_answer_ABC;
    for(int i=0 ; i<answer.size() ; i++){
	 	//temp =  binary_2_ABC(answer[i]);
        final_answer_ABC.push_back(binary_2_ABC(answer[i]));
	}
    int literal;

    literal = count_literal(answer ) ;

    cout   << endl << endl;
    cout << "final_answer" <<  endl;
    cout<<".m"<< final_answer_ABC.size();
     cout   << endl << endl;
     for(int i=0 ; i<final_answer_ABC.size() ; i++){
	 	cout << final_answer_ABC[i] ;
        cout   << endl << endl;
	 }

    // output 
    int out_num = 0;
    fout << ".p " << answer_ABC.size() << endl;
    for(int i=0 ; i<answer_ABC.size() ; i++){
        out_num++;
	 	fout << answer_ABC[i] ;
        fout   << endl; 
        if(out_num>=15){
            break;
        }
	 }


    fout   << endl; 
    fout << ".mc " << final_answer_ABC.size() << endl;

    for(int i=0 ; i<final_answer_ABC.size() ; i++){
	 	fout << final_answer_ABC[i] ;
        fout  << endl ;
	 }
     fout  << "literal=" <<literal  ;

    inFile.close();
	fout.close();	
    // cout   << endl << endl;
    // for(int i=0 ; i<used.size() ; i++){
	// 	cout << used[i] << "    ";
	// }

    // cout   << endl << endl;
    // for(int i=0 ; i<Second_Implicant.size() ; i++){
	// 	cout << Second_Implicant[i] << " ";
	// }
      return 0;   
}
