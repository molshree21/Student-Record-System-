#include <iostream>
#include<winsock.h>
#include<windows.h>
#include<sstream>
#include<ctime>
#include<string>
#include<mysql.h>
#include<cstring>
#include<conio.h>
#include<cstring>
#include<map>

using namespace std;
int count_attempt=0;
int curr_user=0;
int user_no=0;
string loggedin_user="";

void showtime()
{
	time_t now = time(0);
	char *dt = ctime(&now);
	cout<<" \t \t \t \t \t Made by Aditi and Arushi " << dt;
	cout<<endl<<endl;
}

class user
{
private:
string username;
string password;
public:

void login()
{
	if(curr_user==1)
	{
	cout<<" \t \t \t \t \t Already logged in";
	return;
	}
	if(count_attempt==3)
	{
		cout<<endl<<endl;
		cout<<" \t \t \t \t \t No more attempts";
		exit(0);
	}
	cout<<"Enter Username and Password to Login"<<endl;
	string user_name;
	string pass_word;

	cout<<"Username:";
	cin>>user_name;
	cout<<"Password:";
	cin>>pass_word;
	cin.ignore();
	system("cls");


	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn ,"localhost" ,"root", "", "studentmanagement" ,0,NULL,0);
	if(conn)
    {
        int qstate = mysql_query(conn,"SELECT username,password FROM userdetail");
        bool flag=false;
        if(!qstate)
        {
            res = mysql_store_result(conn);
            while(row = mysql_fetch_row(res))
                {
                if(row[0]==user_name && row[1]==pass_word)
                {
                        loggedin_user = row[0];
                        cout<<" \t \t \t \t \t \t Login Successful"<<endl;
                        cout<<" \t \t \t \t \t \t WELCOME "<< loggedin_user<<endl;
                        curr_user=1;
                        flag=true;
                        break;
                }
                }
                if(!flag)
                {
                    cout<<" \t \t \t \t \t \t Incorrect Username or Password"<<endl;
                    cout<<" \t \t \t \t \t \t Press Enter and Try again"<<endl;
                    cin.ignore();
                    system("cls");
                    count_attempt++;
                    login();
                }


        }
        else
        {
            cout<<" \t \t \t \t \t No Accounts Registered !";
        }

    }
}

	void logout()
	{
		curr_user=0;
	}
};

class student : public user
{
	long long int sno;
	string firstname, lastname;
	long long int mobile;
	string course, strm;
	public:
	    // This method is protected for admin use only
		void addStudent()
		{
			if(loggedin_user=="")
			{
				cout<<" \t \t \t \t \t Please Login"<<endl;
				return ;
			}
            if(loggedin_user!="admin")
			{
				cout<<" \t \t \t \t \t Not Authorized. Only admin can add new books"<<endl;
				return ;
			}


			cout<<"Enter student roll number  :\n";
			cin>>sno;
            cin.ignore();
			cout<<"Enter student first name: \n";
			cin>>firstname;
            cin.ignore();
			cout<<"Enter last name: \n";
			cin>>lastname;

			cout<<"Enter mobile number \n";
			cin>>mobile;
            cin.ignore();

			cout<<"Enter course name: \n";
			cin>>course;
            cin.ignore();
			cout<<"Enter stream: \n";
			cin>>strm;


            string nul="";
			MYSQL* conn;
			conn = mysql_init(0);
			conn = mysql_real_connect(conn ,"localhost" ,"root", "", "studentmanagement" ,0,NULL,0);
			stringstream ss;
			ss<<"INSERT INTO studentdetails(studentno, firstname, lastname, mobileno, course, stream) VALUES('"<<sno<<"','"<<firstname<<"','"<<lastname<<"','"<<mobile<<"','"<<course<<"','"<<strm<<"')";
			int qstate=0;
			string query =  ss.str(); //isko str me
            const char* q= query.c_str();// isse hume array od char me kar rhe
            qstate = mysql_query(conn,q);
            if(qstate==0)
            {
            cout<<" \t \t \t \t \t Student Record Added Successfully!"<<endl;
            return;
            }
            else
            {
                cout<<" \t \t \t \t \t Failed"<<endl;
                return;
            }

		}

        // This method is used to search for a particular record in the database
		bool searchStudent(string no)
		{
			if(loggedin_user=="")
			{
				 cout<<" \t \t \t \t \t Please Login"<<endl;
				 return false;
			}

			bool flag=false;
			MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"root", "", "studentmanagement" ,0,NULL,0);
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT studentno, firstname, lastname, mobileno, course, stream from studentdetails");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        if(row[0]== no)
                        {
                            cout<<"Student Details are : "<<endl;
                            cout<<row[0]<<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<"  "<<row[4]<<"  "<<row[5]<<endl;
                            flag=true;
                        }
                }
            }
            else cout<<"Failed"<<endl;
            if(!flag)
                {
                    cout<<"Student Details Not Found !"<<endl;
                    return false;
                }
            }
		}

        // This method is used to view all the records in the database
		void allStuddents()
		{
			MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"root", "", "studentmanagement" ,0,NULL,0);
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT studentno, firstname, lastname, mobileno, course, stream from studentdetails");
                if(!qstate)
                {
                    res = mysql_store_result(conn);
                    while(row = mysql_fetch_row(res))
                    {
                            cout<<" \t \t \t \t \t "<<row[0] <<"  "<<row[1]<<"  "<<row[2]<<"  "<<row[3]<<"  "<<row[4]<<"  "<<row[5]<<"  "<<endl;
                    }
                }
            }
            else cout<<"Failed"<<endl;
		}


    // This method is used to update the record of student in database
    void updateStudent()
	{
		if(loggedin_user=="")
		{
			cout<<"Please Login"<<endl;
			return ;
		}
		string sno;
		cout<<" \t Enter student roll number to update"<<endl;
		cin>>sno;
        string fname, lname, crse, sec;
        long long int mobileno;
        cout<<" \t Enter new first name"<<endl;
		cin>>fname;
		cin.ignore();
		cout<<" \t Enter new last name"<<endl;
		cin>>lname;
		cin.ignore();
		cout<<" \t Enter new mobile name"<<endl;
		cin>>mobileno;
		cin.ignore();

		cout<<" \t Enter new course name"<<endl;
		cin>>crse;
		cin.ignore();
		cout<<" \t Enter new section name"<<endl;
		cin>>sec;

            MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"root", "", "studentmanagement" ,0,NULL,0);
            stringstream ss;
            int qstate=0;
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT studentno, firstname, lastname, mobileno, course, stream from studentdetails");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        if(sno==row[0])
                        {
                            ss<<"UPDATE studentdetails SET firstname= '"<<fname<<"', lastname = '"<< lname <<"', mobileno = '"<<mobileno<<"', course = '"<<crse<<"', stream ='"<<sec<<"' WHERE  studentno = "<<sno;
                            string query =  ss.str();
                              const char* q= query.c_str();
                              qstate = mysql_query(conn,q);
                              if(qstate==0){
                                cout<<" \t \t \t \t \t Details Updated Successfully "<<endl;
                                return;
                              }
                              else cout<<"Failed"<<endl;
                              return;
                        }
                }

            }
		}
    }

    // This method is used to delete a record
	void deleteStudent()
	{
		if(loggedin_user=="")
		{
			cout<<"Please Login"<<endl;
			return ;
		}
		string sno;
		cout<<" \t Enter student roll number to be deleted"<<endl;
		cin>>sno;

		MYSQL* conn;
            MYSQL_ROW row;
            MYSQL_RES* res;
            conn = mysql_init(0);
            conn = mysql_real_connect(conn ,"localhost" ,"root", "", "studentmanagement" ,0,NULL,0);
            stringstream ss;
            int qstate=0;
            if(conn)
            {
            int qstate = mysql_query(conn,"SELECT studentno, firstname, lastname, mobileno, course, stream from studentdetails");
            if(!qstate)
            {
                res = mysql_store_result(conn);
                while(row = mysql_fetch_row(res))
                {
                        if(sno==row[0])
                        {
                            ss<<"DELETE FROM studentdetails WHERE studentno="<<sno;
                            string query =  ss.str();
                              const char* q= query.c_str();
                              qstate = mysql_query(conn,q);
                              if(qstate==0){
                                cout<<" \t \t \t \t \t Details Deleted Successfully "<<endl;
                              }
                              else cout<<"Failed"<<endl;
                              return;
                        }
                }
            }
		}

	}
};


int main()
{
    showtime();
	system("Color B5");
    cout<<"\t\t\t=============================================================================="<<endl;
    cout<<"\t\t\t                          Student Record System                            "<<endl;
    cout<<"\t\t\t=============================================================================="<<endl;
	user u1;
	int x;
	int choice=0;
	student b1;
	do
	{
		cout<<endl;
		cout<<"\tMenu:"<<endl;
		cout<<" \t 1. Administrator Login"<<endl;
		cout<<" \t 2. All Students"<<endl;
		cout<<" \t 3. Add New Student Record"<<endl;
		cout<<" \t 4. Search Student Record"<<endl;
		cout<<" \t 5. Delete a Student Record"<<endl;
		cout<<" \t 6. Update Student Record"<<endl;
		cout<<" \t 7. Logout"<<endl;
		cout<<" \t 0 to exit"<<endl;
		cin>>x;
	string no2;
	switch(x)
	{
		case 1 : u1.login();
				 break;
		case 2 : b1.allStuddents();
				 break;
		case 3 : b1.addStudent();
				 break;
		case 4 : cout<<"Enter student roll no";
				 cin>>no2;
				 b1.searchStudent(no2);
				 break;
        case 5 : b1.deleteStudent();
                 break;
		case 6 : b1.updateStudent();
				 break;
		case 7:  u1.logout();
				 break;
		case 0 : choice=1;

	}
	}
	while(choice==0);

}
