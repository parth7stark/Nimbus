//main server


#define MAXFILESIZE 512
#define MAXBUFFERSIEZ 512

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iostream>
#include <fstream>

#include <map>
#include<vector>
#include <my_global.h>
#include <mysql.h>
#include "proto.h"
// #include "parser1.h"  //also function command parser
using namespace std;
string SERVERPATH = "/home/i7-workstation/Desktop/mainpro/cannismajor/freshstrt/serverfile/";


struct mysqlrturndata
{
  int SIGN_IN_FLAG ;
  int CURRENTUID ;
  string CURRENT_USERNAME ;
  float data;
};

struct dataset
{
   string info1;
   string info2;
   string command_name;
   int status = 0;
};


typedef struct ClientNode {
    int data;
    struct ClientNode* prev;
    struct ClientNode* link;
    char ip[16];
    char name[31];
} ClientList;

ClientList *newNode(int sockfd, char* ip) {
    ClientList *np = (ClientList *)malloc( sizeof(ClientList) );
    np->data = sockfd;
    np->prev = NULL;
    np->link = NULL;
    strncpy(np->ip, ip, 16);
    strncpy(np->name, "NULL", 5);
    return np;
}
int server_sockfd = 0, client_sockfd = 0;

ClientList *root, *now;





void stringtochar(char arra[],string ar1)
{
  int length = ar1.length();
  // std::cout << "Thats the wau i likeit" << '\n';
  for (int i = 0; i < length; i++)
  {
    arra[i] = ar1[i];
    printf("%c",arra[i]);

  }
  arra[length] = '\0';
}

string chartostring(char arra[],int n0)
{
  string str1="";
  int i;
  for ( i = 0; i!=n0; i++)
  {
    str1 += arra[i];
  }
// str1 += '\0';
return str1;
}



void SHOW_BASE_DES()
{
  sleep(5);
  system("clear");
  std::cout << "----------------------------WELCOME TO DROPSHIT-----------------------" << '\n'<<endl<<endl;

}






















//commanding parsing










vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
      return tokens;
}

dataset parser(string command)
{
//    string command;
    cout << "the commands is "<<command<<endl;
//    getline (cin, command);
    command+=" ";
    vector<string> words;
    words = split(command," ");
    int n = words.size();
    // std::cout << "  n"<<n << '\n';
    // for (int i = 0;i<n;i++)
    // {
    //     cout << words[i] << endl;
    // }
    // cout<<"1"<<endl;
    // datset initialisation
    dataset ds;
    //for sign_up set up
    if(words[0].compare("sign_up")==0)
    {

        ds.info1 = words[1];
        ds.info2 = words[2];
        ds.command_name = "sign_up";
        ds.status = 1;
    }
    // else if(words[0].compare("sign_in")==0)
    // {
    //
    //     ds.info1 = words[1];
    //     ds.info2 = words[2];
    //     ds.command_name = "sign_in";
    //     ds.status = 1;
    // }
    else if(words[0].compare("upload")==0)
    {

        ds.info1 = words[1];
        ds.info2 = words[2];
        ds.command_name = "upload";
        ds.status = 1;
    }
    else if(words[0].compare("download")==0)
    {

        ds.info1 = words[1];
        ds.info2 = words[2];
        ds.command_name = "download";
        ds.status = 1;
    }
    else if(words[0].compare("sign_in")==0)
    {
        dataset cr;
        ds.info1 = words[1];
        ds.info2 = words[2];
        ds.command_name = "sign_in";
        ds.status = 1;
    }
    else if(words[0].compare("logout")==0)
    {
        dataset cr;
        ds.info1 = "null";
        ds.info2 = "null";
        ds.command_name = "logout";
        ds.status = 1;
    }
    else if(words[0].compare("view")==0)
    {
        dataset cr;
        ds.info1 = "null";
        ds.info2 = "null";
        ds.command_name = "view";
        ds.status = 1;
    }
    else if(words[0].compare("delete")==0)
    {
        dataset cr;
        ds.info1 = words[1];
        ds.info2 = "null";
        ds.command_name = "delete";
        ds.status = 1;
    }
    else{
        printf("ERROR: command not found\n");
    }
    return ds;
}




//mysql treatment









void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}




string initialview()
{
  // std::cout << "this is a  test" << '\n';
  // std::cin >> tam;

  string str1="";


  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "user12", "34klq*", "nimbus", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "SELECT Filename FROM FileDB where Flag = 0;"))
  {
      finish_with_error(con);
  }
  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL)
  {
      finish_with_error(con);

  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;

  while ((row = mysql_fetch_row(result)))
  {
      for(int i = 0; i < num_fields; i++)
      {
          // printf("%s ", row[i] ? row[i] : "NULL");
          str1+= row[i] ? row[i] : "NULL";

      }
          // printf("\n");
          str1+="\n";
  }
   mysql_free_result(result);
  mysql_close(con);

  return str1;

}










int recv_throughsocket(char arr1[],int lenght,int sockfd1)
{
  // send(sockfd, inoput, sizecommand1*sizeof(char), 0);
  int i = 0;
  char c123;
  while (i<lenght)
  {

    recv(sockfd1,&c123,sizeof(char),0);
    arr1[i] = c123;
    i++;

  }
  arr1[i] = '\0';
  return 0;

}



int send_throughsocket(char arr1[],int lenght,int sockfd1)
{
  // send(sockfd, inoput, sizecommand1*sizeof(char), 0);
  int i = 0;
  char c123;
  while (i<lenght)
  {
    c123 = arr1[i];
    // std::cout << "Sneding "<<c123<<":" << '\n';
    send(sockfd1,&c123,sizeof(char),0);
    i++;

  }

return 0;

}





string gotomyslbinder(dataset ds,mysqlrturndata* ds2,int sockfd12)
// struct mysqlrturndata gotomyslbinder(dataset ds)
{

MYSQL *con = mysql_init(NULL);
string str1="";
int c1;
if (con == NULL)
{
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
}
if (mysql_real_connect(con, "localhost", "user12", "34klq*", "nimbus", 0, NULL, 0) == NULL)
{
    finish_with_error(con);
}

    string c_name = ds.command_name;
    if(c_name.compare("upload")==0)
    {

          if(ds.status == 1)
          {
            printf("\nYou have reached upload\n" );


      string Filename1 = ds.info1; //pass this string as arguments. comment this
      string Fileformat = ds.info2; //pass this string as arguments
      std::cout << Filename1 << '\n';
      std::cout << Fileformat << '\n';
      Filename1 = Filename1+"."+Fileformat;
      std::cout << Filename1 << '\n';
      // string query = "SELECT Username, Password FROM UserDB WHERE Username =\"" + Username + "\" AND Password = \""+Password+"\" " ;
      string query = "SELECT Filename,Fileptr FROM FileDB WHERE Filename =\"" + Filename1 + "\"";

              if (mysql_query(con, query.c_str()))
              // if (mysql_query(con, queryinchar1))
              {
                  finish_with_error(con);
              }

              MYSQL_RES *result = mysql_store_result(con);

              // query="";
              if (result == NULL)
              {
                  finish_with_error(con);
              }

              int num_fields = mysql_num_fields(result);
              string uid;
              MYSQL_ROW row;

              int found=0;    //0 means not in table

              while ((row = mysql_fetch_row(result)))
              {
                  // cout<<"\n user: "<<row[0];       //printing table

                  if((strcmp(row[0],Filename1.c_str())==0) )
                  {
                      found++;
                  }
              }
              send(sockfd12,&found,sizeof(int),0);

              if(found==1)    //if found in table set signin flag = 1;
              {
                  str1 = "The filename already exists ";
                  std::cout << Filename1 << '\n';
                  std::cout << found << '\n';

              }
              else
                  {
                    str1 = "\n File is being added \n ";

                    // FILE *fp2 ;
                    string path = SERVERPATH+Filename1;
                    std::cout << "path of server "<<path << '\n';
                    char path1[path.length()+1] ;
                    strcpy(path1,path.c_str());
                    int success1 = -1;
                    int bytesRead, bytesWritten = 0;
                     fstream f2;
                   // f2.open("New.mp4", ios::out);
                   recv(sockfd12,&success1,sizeof(int),0);
                   if (success1)
                   {
                    std::cout << "Failed "<<" No such file" << '\n';
                    str1 = "Wrong path no such file";
                   }
                  else
                   {
                  f2.open(path1, ios::out);

                   int ch23;
                   while((recv(sockfd12, &ch23, sizeof(ch23), 0)>0))
                   {

                  printf("%c\n",ch23 );
                  printf("%d\n",ch23 );

                  if (ch23==-1)
                  {
                    printf("higaniga%c",ch23 );
                      break;
                  }
                   f2.put(ch23);

                   }
                   cout<<"\n file received \n";
                   f2.close();










                    // fprintf(fp2,"%s",buffer);
                    // int recvdataforfiellen = 0;
                    // recv(sockfd12,&recvdataforfiellen,sizeof(int),0);
                    // char dataforfiel[recvdataforfiellen+1];
                    // recv_throughsocket(dataforfiel,recvdataforfiellen,sockfd12);
                    // fp2 = fopen(path1,"w");
                    // while (read(sockfd12,&c1,sizeof(int)))




/* Getting file size */
                    FILE* fp = fopen(path1, "r");

                    // // checking if the file exist or not
                    // if (fp == NULL) {
                    // printf("File Not Found!\n");
                    // return -1;
                    // }

                    fseek(fp, 0L, SEEK_END);

                    // calculating the size of the file
                    // long int filesiez23 = ftell(fp);
                    float filesiez23 = ftell(fp);

                    // closing the file
                    fclose(fp);

                    string fielsiez23s = to_string(filesiez23);

                    string uidforsql = to_string(ds2->CURRENTUID);

                    std::cout << "\n\n\n File size\n" << '\n';
                    std::cout << filesiez23 << '\n';
                    // {
                    //   // fwrite(ci,sizeof(c),1,fp2);
                    //   fputc(c1,fp2);
                    //   // printf("%c\n",c1 );
                    //   if (c1==13)
                    //   {
                    //     break;
                    //   }
                    // }
                    // std::cout << "check data" << '\n';
                    // std::cout << dataforfiel << '\n';
                    // for (int i = 0; i < recvdataforfiellen; i++)
                    // {
                    //   c1 = dataforfiel[i];
                    //     fputc(c1,fp2);
                    //
                    // }
                    //
                    //
                    string query2 = "";
                    float new_mem = 0;
                    string new_mems = "";
                    // fclose(fp2);
                    //INSERT INTO FileDB (Filename,Size,uid,Flag,Fileptr) VALUES ("144.txt",56,3,1,"9.txt");
                    if (ds2->SIGN_IN_FLAG == 1)
                    {
                        query = "INSERT INTO FileDB(Filename,Size,uid,Flag,Fileptr) VALUES (\"" + Filename1 + "\","+fielsiez23s+", "+uidforsql+",1,\"" + path +"\")";
                        new_mem = ds2->data-filesiez23;
                        std::cout << filesiez23 << '\n';
                        ds2->data = new_mem;
                        new_mems = to_string(new_mem);
                        query2 = "UPDATE UserDB set Memory_left =" +new_mems +"where uid = "+uidforsql;
                        if (mysql_query(con, query2.c_str()))
                        // if (mysql_query(con, queryinchar1))
                        {
                            finish_with_error(con);
                        }


                   }
                    else
                    {
                        query = "INSERT INTO FileDB(Filename,Size,Flag,Fileptr) VALUES (\"" + Filename1 + "\","+fielsiez23s+", 0,"+"\""+path+"\")";


                    }

                            if (mysql_query(con, query.c_str()))
                            // if (mysql_query(con, queryinchar1))
                            {
                                finish_with_error(con);
                            }







                          }



                  }

                  mysql_free_result(result);

              //     std::cout << "username" <<ds2->CURRENT_USERNAME<< '\n';
              //     std::cout << "Userid" <<ds2->CURRENTUID<< '\n';
              // int tempstatus = ds2->SIGN_IN_FLAG;
              // send(sockfd12,&tempstatus,sizeof(int),0);
              // int tempstringlegth = Username.length();
              // send(sockfd12,&tempstringlegth,sizeof(int),0);
              // char tempstring12[tempstringlegth+1];
              // strcpy(tempstring12,Username.c_str());
              // std::cout << " \n test "<<tempstring12 << '\n';
              // send_throughsocket(tempstring12,tempstringlegth,sockfd12)     ;




          }

    }
    else if(c_name.compare("download")==0)
    {
      if(ds.status == 1)
      {
        printf("\nYou have reached download agian\n" );


  string Filename1 = ds.info1; //pass this string as arguments. comment this
  string Fileformat = ds.info2; //pass this string as arguments
  std::cout << Filename1 << '\n';
  std::cout << Fileformat << '\n';
  Filename1 = Filename1+"."+Fileformat;
  std::cout << Filename1 << '\n';
  // string query = "SELECT Username, Password FROM UserDB WHERE Username =\"" + Username + "\" AND Password = \""+Password+"\" " ;
  if(ds2->SIGN_IN_FLAG!=1)
    {

        string query = "SELECT Filename,Fileptr FROM FileDB WHERE Filename =\"" + Filename1 + "\"";

          if (mysql_query(con, query.c_str()))
          // if (mysql_query(con, queryinchar1))
          {
              finish_with_error(con);
          }

          MYSQL_RES *result = mysql_store_result(con);

          // query="";
          if (result == NULL)
          {
              finish_with_error(con);
          }

          int num_fields = mysql_num_fields(result);
          string uid;
          MYSQL_ROW row;
          string fileptr = "";
          int found=0;    //0 means not in table
          int tempo =-1;



          while ((row = mysql_fetch_row(result)))
          {
              // cout<<"\n user: "<<row[0];       //printing table

              if((strcmp(row[0],Filename1.c_str())==0) )
              {
                  found++;

              }
              fileptr = row[1];
          }

          // send(sockfd12,&found,sizeof(int),0);
          std::cout << "check file pointer "<<fileptr << '\n';
          if (fileptr=="")
          {
            printf("No such file \n");
            str1 = "No such file\n";
            tempo = 1;
          }

          else
          {
            tempo =0;
            str1 = "\n File is sent  \n ";
            printf("files found\n");
            string fielname = fileptr;
            char fielnae1[fielname.length()+1] ;
            strcpy(fielnae1,fielname.c_str());


            fstream f1;
            int ch23;
            int bytesRead, bytesWritten = 0;

            f1.open(fielnae1, ios::in | ios::binary);
            if (tempo==0)
            {
              tempo = f1.fail();
              printf("check tempo before opening %d",tempo );
            }
            else
            {
              tempo = 1;
            }
            send(sockfd12,&tempo,sizeof(int),0);
            // recv(sockfd12,&success1,sizeof(int),0);


            if (tempo)
            {
                      std::cout << "No such file" << '\n';
            }

            else
            {

                while(!f1.eof())
                {

                 ch23 = f1.get();

                 bytesWritten +=send(sockfd12, &ch23, sizeof(ch23),0); //strlen(ch23)

               }
               // ch23 = 3;
               // send(sockfd, &ch23, sizeof(ch23),0); //strlen(ch23)
                cout<<"\n File sended successfully. \n";

            }


            f1.close();

          }

                 mysql_free_result(result);



       }


       else
       {
         int udi12 ;
         string udi13 = "-2";

           string query = "SELECT Filename,Fileptr ,uid FROM FileDB WHERE Filename =\"" + Filename1 + "\"";

             if (mysql_query(con, query.c_str()))
             // if (mysql_query(con, queryinchar1))
             {
                 finish_with_error(con);
             }

             MYSQL_RES *result = mysql_store_result(con);

             // query="";
             if (result == NULL)
             {
                 finish_with_error(con);
             }

             int num_fields = mysql_num_fields(result);
             string uid;
             MYSQL_ROW row;
             string fileptr = "";
             int found=0;    //0 means not in table
             int tempo =-1;



             while ((row = mysql_fetch_row(result)))
             {
                 // cout<<"\n user: "<<row[0];       //printing table

                 if((strcmp(row[0],Filename1.c_str())==0) )
                 {
                     found++;

                 }
                 fileptr = row[1];
                 // udi12 = stoi(row[2]);
                 udi13 = (row[2]);
             }
             // printf("udi13 = %d\n",udi13 );
             std::cout << udi13 << '\n';
             // send(sockfd12,&found,sizeof(int),0);
             std::cout << "check file pointer "<<fileptr << '\n';
             if( stoi(udi13)==ds2->CURRENTUID)
             {
               if (fileptr=="")
                 {
                   printf("No such file \n");
                   str1 = "No such file\n";
                   tempo = 1;
                 }

                 else
                 {
                   tempo =0;
                   str1 = "\n File is sent  \n ";
                   printf("files found\n");
                   string fielname = fileptr;
                   char fielnae1[fielname.length()+1] ;
                   strcpy(fielnae1,fielname.c_str());


                   fstream f1;
                   int ch23;
                   int bytesRead, bytesWritten = 0;

                   f1.open(fielnae1, ios::in | ios::binary);
                   if (tempo==0)
                   {
                     tempo = f1.fail();
                     printf("check tempo before opening %d",tempo );
                   }
                   else
                   {
                     tempo = 1;
                   }
                   send(sockfd12,&tempo,sizeof(int),0);
                   // recv(sockfd12,&success1,sizeof(int),0);


                   if (tempo)
                   {
                             std::cout << "No such file" << '\n';
                   }

                   else
                   {

                       while(!f1.eof())
                       {

                        ch23 = f1.get();

                        bytesWritten +=send(sockfd12, &ch23, sizeof(ch23),0); //strlen(ch23)

                      }
                      // ch23 = 3;
                      // send(sockfd, &ch23, sizeof(ch23),0); //strlen(ch23)
                       cout<<"\n File sended successfully. \n";

                   }


                   f1.close();

                 }

              }

              else
              {
                printf("No such file \n");
                str1 = "No such file\n";
              }


               mysql_free_result(result);



          }


      }
    }
    else if(c_name.compare("sign_up")==0)
    {
        if(ds.status == 1)
        {
            /*
            enter the query here for sign_up
            */
            string Username = ds.info1; //pass this string as arguments. comment this
            string Password = ds.info2; //pass this string as arguments
            // string Password="";
            cout<<"username received "<<Username<<endl;
            cout<<"Password received "<<Password<<endl;



            string query = "SELECT Username FROM UserDB WHERE Username =\"" + Username + "\"";
             //cout<<"\n query: "<<query;    checking if query is appended properly
             if (mysql_query(con, query.c_str()))
             {
                 finish_with_error(con);
             }

             MYSQL_RES *result = mysql_store_result(con);

             query="";
             if (result == NULL)
             {
                 finish_with_error(con);
             }

             int num_fields = mysql_num_fields(result);

             MYSQL_ROW row;

             int found=0;    //0 means not in table

             while ((row = mysql_fetch_row(result)))
             {
                 // cout<<"\n user: "<<row[0];       //printing table

                 if(strcmp(row[0],Username.c_str())==0)
                 {
                     found=1;
                     break;
                 }
             }

             if(found==0)
             {
                 // string Password = "123";    //comment this when you pass arguments

                 //enter username and Password in table

                 query="INSERT INTO UserDB(Username, Password, Memory_left) VALUES(\""+ Username + "\",\"" + Password + "\"," + "1000)";

                 // cout<<"\n query: "<<query;

                 if(mysql_query(con,query.c_str()))
                 {
                     finish_with_error(con);

                 }
                 //see if inserted in table or not
                 str1 = Username+ " Added successfully \n 100 MB\n";

             }
             else
                 {
                   cout<<"\n Aready exists.\n ";
                   str1 = "\n Aready exists.\n ";

                  }



                  mysql_free_result(result);






        }
    }
    else if(c_name.compare("sign_in")==0)
    {
        if(ds.status == 1)
        {
            /*
            enter the query here for sign_in
            */

    string Username = ds.info1; //pass this string as arguments. comment this
    string Password = ds.info2; //pass this string as arguments
    // string Password="";
    // cout<<"username received "<<Username<<endl;
    // cout<<"Password received "<<Password<<endl;
    // std::cout << Username.length() << '\n';
    // std::cout << Password1.length() << '\n';

    // Password[Password.length()-1] = '\0';
    // for (int i = 0; i < Username.length(); i++)
    // int i10 = 0;
    // while (i10<Password1.length()-1)
    // {
    //
    //   Password += Password1.at(i10);
    //   // std::cout << Password.at(i10) << '\n';
    //   std::cout << Password1.at(i10) << '\n';
    //   std::cout << "i" <<i10<< '\n';
    //
    //   i10++;
    //
    // }
    // std::cout << Username.length() << '\n';
    // std::cout << Password.length() << '\n';

    // string query = "SELECT Username, Password FROM UserDB WHERE Username =\"" + Username + "\" AND Password = \""+Password+"\" " ;
    string query = "SELECT Username, Password ,uid,Memory_left FROM UserDB WHERE Username =\"" + Username + "\" AND Password = \""+Password+"\" " ;

            // char queryinchar1[query.length()+1];
            // cout<<"\nSql query:"<<query<<endl;
            // Username = "Brosh_21";
            // Password = "Birth12";
           // strncpy(queryinchar1,query.c_str(),query.length());
            // stringtochar(queryinchar1,query);
            // queryinchar1[query.length()] = '\0';

            // cout<<"\nSql query in character:"<<queryinchar1<<endl;
            // printf("\n Sql query in char %s",queryinchar1);

            if (mysql_query(con, query.c_str()))
            // if (mysql_query(con, queryinchar1))
            {
                finish_with_error(con);
            }

            MYSQL_RES *result = mysql_store_result(con);

            // query="";
            if (result == NULL)
            {
                finish_with_error(con);
            }

            int num_fields = mysql_num_fields(result);
            string uid;
            MYSQL_ROW row;

            int found=0;    //0 means not in table

            while ((row = mysql_fetch_row(result)))
            {
                // cout<<"\n user: "<<row[0];       //printing table

                if((strcmp(row[0],Username.c_str())==0) && (strcmp(row[1],Password.c_str())==0))
                {
                    found=1;
                    uid = row[2];
                    ds2->CURRENTUID = stoi(uid);
                    ds2->data = stof(row[3]);
                    break;
                }
            }

            if(found==1)    //if found in table set signin flag = 1;
            {
                ds2->SIGN_IN_FLAG = 1;
//                cout<<"\n Logged in. ";
                str1 = "The username "+ Username +" has successfully logged in ";
                std::cout << str1 << '\n';
                ds2->CURRENT_USERNAME = Username;
            }
            else
                {
                  str1 = "\n Incorrect Username or Password.\n ";
                  cout<<"\n Incorrect Username or Password.\n ";
                }

                mysql_free_result(result);

                std::cout << "username" <<ds2->CURRENT_USERNAME<< '\n';
                std::cout << "Userid" <<ds2->CURRENTUID<< '\n';
            int tempstatus = ds2->SIGN_IN_FLAG;
            send(sockfd12,&tempstatus,sizeof(int),0);
            float meory = ds2->data;
            send(sockfd12,&meory,sizeof(float),0);

            int tempstringlegth = Username.length();
            send(sockfd12,&tempstringlegth,sizeof(int),0);
            char tempstring12[tempstringlegth+1];
            strcpy(tempstring12,Username.c_str());
            std::cout << " \n test "<<tempstring12 << '\n';
            send_throughsocket(tempstring12,tempstringlegth,sockfd12)     ;
        }
    }
    else if(c_name.compare("delete")==0)
    {
      int uid1;
      if(ds.status == 1)
      {
          /*
          enter the query here for sign_up
          */
          string filename1 = ds.info1; //pass this string as arguments. comment this
          if(ds2->SIGN_IN_FLAG==1)
          {

          // string query = "DELETE FROM FileDB WHERE Filename =\"" + filename1 + "\"";
          string query = "SELECT uid FROM FileDB WHERE Filename =\"" + filename1 + "\"";

           //cout<<"\n query: "<<query;    checking if query is appended properly
           if (mysql_query(con, query.c_str()))
           {
               finish_with_error(con);
           }

           MYSQL_RES *result = mysql_store_result(con);

           query="";
           if (result == NULL)
           {
               finish_with_error(con);
           }

           int num_fields = mysql_num_fields(result);

           MYSQL_ROW row;

           int found=0;    //0 means not in table

           while ((row = mysql_fetch_row(result)))
           {
               // cout<<"\n user: "<<row[0];       //printing table

               if (row[0]=="NULL")
               {
                 uid1 = -1; // public files

               }
               else
               {
                 uid1 = stoi(row[0]);
               }

           }

           // if (uid1 == -1)
           // {
           //
           //   query = "DELETE FROM FileDB WHERE Filename =\"" + filename1 + "\"";
           //
           //
           //       if (mysql_query(con, query.c_str()))
           //       {
           //           finish_with_error(con);
           //       }
           //
           //       MYSQL_RES *result1 = mysql_store_result(con);
           //
           //       // query="";
           //       if (result1 == NULL)
           //       {
           //           finish_with_error(con);
           //       }
           //
           //       int num_fields1 = mysql_num_fields(result1);
           //       MYSQL_ROW row1;
           //
           //       int found1=0;    //0 means not in table
           //
           //       while ((row1 = mysql_fetch_row(result1)))
           //       {
           //           // cout<<"\n user: "<<row[0];       //printing table
           //
           //           if((strcmp(row1[0],Username.c_str())==0) && (strcmp(row[1],Password.c_str())==0))
           //           {
           //               found=1;
           //               uid = row[2];
           //               ds2->CURRENTUID = stoi(uid);
           //               break;
           //           }
           //       }
           //
           //       if(found==1)    //if found in table set signin flag = 1;
           //       {
           //           ds2->SIGN_IN_FLAG = 1;
           //           str1 = "The username "+ Username +" has successfully logged in ";
           //           std::cout << str1 << '\n';
           //           ds2->CURRENT_USERNAME = Username;
           //       }
           //       else
           //           {
           //             str1 = "\n Incorrect Username or Password.\n ";
           //             cout<<"\n Incorrect Username or Password.\n ";
           //           }
           //
           //           mysql_free_result(result);
           //
           //           std::cout << "username" <<ds2->CURRENT_USERNAME<< '\n';
           //           std::cout << "Userid" <<ds2->CURRENTUID<< '\n';
           //
           //
           //
           //
           //
           //
           // }
           //
           // else
           // {
           //    query = "DELETE FROM FileDB WHERE Filename =\"" + filename1 + "\" and uid = "+ds2->CURRENTUID ;
           //
           //
           // }


                mysql_free_result(result);


//delete the path as well

          }//SIGN IN TRUE



      }
    }
    else if(c_name.compare("view")==0)
    {

        if(ds.status == 1)
        {
          printf("Reached ehroihflksdng\n" );

          if (mysql_query(con, "SELECT Filename FROM FileDB where Flag = 0 ;"))
          {
              finish_with_error(con);
          }


          MYSQL_RES *result = mysql_store_result(con);

          if (result == NULL)
          {
              finish_with_error(con);

          }

          int num_fields = mysql_num_fields(result);

          MYSQL_ROW row;

          while ((row = mysql_fetch_row(result)))
          {
              for(int i = 0; i < num_fields; i++)
              {
                  // printf("%s ", row[i] ? row[i] : "NULL");
                  str1+= row[i] ? row[i] : "NULL";

              }
                  // printf("\n");
                  str1+="\n";
          }

          //harman update view manager

          // return str1;
          // printf("%s\n",str1 );
            // std::cout << str1 << '\n';

            // send case
            // int templenght = str1.length();
            // char array1[templenght+1] = {"lunda"};
            // strncpy(array1,str1.c_str(),templenght);
            // std::cout << "teja machuda later" << '\n';
            // std::cout << array1 << '\n';
            // send(localsocket,array1,MAXBUFFERSIEZ,0);

            // std::cout << "Selvarak" << '\n';


          //check status whethe signed it

          mysql_free_result(result);


          string query = "SELECT Filename FROM FileDB WHERE Flag = 1 AND uid = "+to_string(ds2->CURRENTUID) ;

          //private Files
      if(ds2->SIGN_IN_FLAG == 1)
        {
          // str1+="\n\n\n\nPrivate.txt\n";
          if (mysql_query(con, query.c_str()))
          {
              finish_with_error(con);
          }


          MYSQL_RES *result = mysql_store_result(con);

          if (result == NULL)
          {
              finish_with_error(con);

          }

          int num_fields = mysql_num_fields(result);

          MYSQL_ROW row;

          while ((row = mysql_fetch_row(result)))
          {
              for(int i = 0; i < num_fields; i++)
              {
                  // printf("%s ", row[i] ? row[i] : "NULL");
                  str1+= row[i] ? row[i] : "NULL";

              }
                  // printf("\n");
                  str1+="\n";
          }



          mysql_free_result(result);


          }









        }
    }

    else if(c_name.compare("logout")==0)
    {
      if (ds2->SIGN_IN_FLAG == 0)
      {
        str1 = "\nNo Login\n\n";
        std::cout << str1 << '\n';

      }
      else
      {
        str1 = "\nLogged out \n"+ds2->CURRENT_USERNAME;
        ds2->SIGN_IN_FLAG = 0;
      }
      int temp34 = ds2->SIGN_IN_FLAG ;
      send(sockfd12,&temp34,sizeof(int),0);
    }
    else
    {
        printf("NOT FOUND\n");
    }
     mysql_close(con);
     // printf("\nds;foighreached");
     return str1; //return whatever the answer query is

}
























void *client_handler(void *p_client)
{
    // char menu[] = {"1. This is a sample text where all public files are supposed be here\n"};
    char menu[1000];
    int SIGN_IN_FLAG = 0 ;
    int CURRENTUID = -1;
    string CURRENT_USERNAME = "";
    // int menulenght = 1;
    struct mysqlrturndata recevfromsql;
    recevfromsql.CURRENT_USERNAME = "";
    recevfromsql.CURRENTUID = 0;
    recevfromsql.SIGN_IN_FLAG = 0;

    int leave_flag = 0;
    int option_recv = -1;
    char nickname[LENGTH_NAME] = {};
    char recv_buffer[LENGTH_NAME] = {};
    char send_buffer[LENGTH_SEND] = {};
    char test12[] ={'a'};
    test12[0] = 12;
    ClientList *np = (ClientList *)p_client;
    string sendtofu,intialsenddata,sendatatoserver;
    char sendmainmessage[MAXBUFFERSIEZ];
    struct dataset parsedcommand;
    int i1 = 0,sendatatoserverlen=0;
    int commandlenght1 = 0;
    int lengthofmenu = 0;
    printf("\nReached here bruh\n" );
    intialsenddata = initialview();//tamper with menu
      strcpy(menu, intialsenddata.c_str());
      lengthofmenu = strlen(menu);
      send(np->data,&lengthofmenu,sizeof(int),0);
      // send(np->data,menu,sizeof(menu),0);
      send_throughsocket(menu,lengthofmenu,np->data);

    while (1)
    {
        // recv(np->data,recv_buffer,sizeof(recv_buffer),0);
        recv(np->data,&commandlenght1,sizeof(int),0);

        // recv(np->data,recv_buffer,sizeof(char)*commandlenght1,0);
        recv_throughsocket(recv_buffer,commandlenght1,np->data);
        printf("Command recieved  %s of length %d \n", recv_buffer ,commandlenght1);
        if(strlen(recv_buffer)>=4)
        {
        if(recv_buffer[0]=='e' && recv_buffer[1]=='x' && recv_buffer[2]=='i' && recv_buffer[3]=='t' )
          {
            printf("Bye\n" );
            break;
          }

        }
        else
        {
          continue;
        }
        sendtofu = chartostring(recv_buffer,strlen(recv_buffer));
        // std::cout << "Recieved buffer " <<sendtofu<< '\n';
        parsedcommand = parser(sendtofu);
        // std::cout << "1" <<parsedcommand.info1<< '\n';
        // std::cout << "2" <<parsedcommand.info2<< '\n';
        // std::cout << "3" <<parsedcommand.command_name<< '\n';
        // parsedcommand.command_name+=" ";
        sendatatoserver = gotomyslbinder(parsedcommand,&recevfromsql,np->data);//execute


        std::cout << "String :"<<sendatatoserver << '\n';
        // @test begins
            // strncpy(sendmainmessage,sendatatoserver.c_str(),sendatatoserver.length());
            strcpy(sendmainmessage,sendatatoserver.c_str());

            // for ( int i = 0; i < sendatatoserver.length(); i++)//same as below for loop
        // for ( int i = 0; sendmainmessage[i]!='\0'; i++)
        // {
        //       printf("i= %d  sendmainmessage= %d sendmainmessage= %c  \n",i,sendmainmessage[i],sendmainmessage[i] );
        //
        // }
        // std::cout << "Character:"<<sendmainmessage << '\n';
        if (parsedcommand.command_name=="view")
        {
          printf("\nChecked reaahces\n" );
          fflush(stdin);
          fflush(stdout);
{
        //
        //     // strncpy(sendmainmessage,sendatatoserver.c_str(),sendatatoserver.size());
            // strncpy(sendmainmessage,sendatatoserver.c_str(),sendatatoserver.length());
        //
        //     // sendmainmessage[0] = "h";sendmainmessage[1] = "o";
        //     // std::cin >> sendmainmessage;
        //     // std::cout << sendmainmessage << '\n';
        //     sendatatoserverlen = sendatatoserver.length();
        //     std::cout << "The length of sgring "<<sendatatoserverlen << '\n';
        //     send(np->data,&sendatatoserverlen,sizeof(int),0);
        //
        //     // write(np->data,test12,LENGTH_NAME);
        //
        //     while ((c12 = sendmainmessage[i1++])!='\0')
        //     {
        //       std::cout << "Tset"<<c12 << '\n';
        //       // send(np->data,&c12,sizeof(char),0);
        //       write(np->data,&c12,sizeof(char));
        //
        //
        //     }
        //     // write(np->data,)
        //     printf("again check\n" );
        //
        //

        // if(ifyoufail()==1){dont();}


        //clear buffer

        // send(np->data,&c12,sizeof(char),0);
        // send(np->data,&sendmainmessage,sizeof(sendmainmessage),0);
      }
        sendatatoserverlen = strlen(sendmainmessage);
        // send(np->data,&sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
        // send(np->data,&sendatatoserverlen,sizeof(int),0);
        // send(np->data,&sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);


        send(np->data,&sendatatoserverlen,sizeof(int),0);
        // send(np->data,sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
        send_throughsocket(sendmainmessage,sendatatoserverlen,np->data);
        printf("%s\n",sendmainmessage );

        // send(np->data,test12,sizeof(char*),0);
      }
      else if (parsedcommand.command_name=="sign_in")
      {
        printf("\nChecked reaahcfgfdges\n" );
        // fflush(stdin);
        // fflush(stdout);


      //clear buffer
      std::cout << sendmainmessage << '\n';
      // send(np->data,&c12,sizeof(char),0);
      // send(np->data,&sendmainmessage,MAXBUFFERSIEZ,0);
      // send(np->data,&sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
      sendatatoserverlen = strlen(sendmainmessage);
      // send(np->data,&sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
      send(np->data,&sendatatoserverlen,sizeof(int),0);
      // send(np->data,sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
      send_throughsocket(sendmainmessage,sendatatoserverlen,np->data);
      // send(np->data,test12,sizeof(char*),0);
    }

    else if (parsedcommand.command_name=="sign_up")
    {
      // fflush(stdin);
      // fflush(stdout);


    //clear buffer
    std::cout << sendmainmessage << '\n';
    // send(np->data,&c12,sizeof(char),0);
    // send(np->data,&sendmainmessage,MAXBUFFERSIEZ,0);
    // send(np->data,&sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
    sendatatoserverlen = strlen(sendmainmessage);
    // send(np->data,&sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
    send(np->data,&sendatatoserverlen,sizeof(int),0);
    // send(np->data,sendmainmessage,strlen(sendmainmessage)*sizeof(char),0);
    send_throughsocket(sendmainmessage,sendatatoserverlen,np->data);
    // send(np->data,test12,sizeof(char*),0);
    }


    else if (parsedcommand.command_name=="logout")
    {
      sendatatoserverlen = strlen(sendmainmessage);
      send(np->data,&sendatatoserverlen,sizeof(int),0);
      send_throughsocket(sendmainmessage,sendatatoserverlen,np->data);
    }

    else if (parsedcommand.command_name=="upload")
    {
        std::cout << "Reached upload" << '\n';

    }
    else if (parsedcommand.command_name=="download")
    {
        std::cout << "Reached download" << '\n';

    }
    else if (parsedcommand.command_name=="delete")
    {
      sendatatoserverlen = strlen(sendmainmessage);
      send(np->data,&sendatatoserverlen,sizeof(int),0);
      send_throughsocket(sendmainmessage,sendatatoserverlen,np->data);
    }


    }




    pthread_exit(0);


}




int main(int argc, char const *argv[])
{


    server_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (server_sockfd == -1)
    {
        printf("Faile\n" );
        exit(EXIT_FAILURE);

    }


    struct sockaddr_in server_info, client_info;
    int s_addrlen = sizeof(server_info);
    int c_addrlen = sizeof(client_info);
    memset(&server_info, 0, s_addrlen);
    memset(&client_info, 0, c_addrlen);
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = INADDR_ANY;
    server_info.sin_port = htons(8888);



    bind(server_sockfd, (struct sockaddr *)&server_info, s_addrlen);
    listen(server_sockfd, 50);

    getsockname(server_sockfd, (struct sockaddr*) &server_info, (socklen_t*) &s_addrlen);

    printf("Start Server on: %s:%d\n", inet_ntoa(server_info.sin_addr), ntohs(server_info.sin_port));

    while (1)
    {
        client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_info,(socklen_t*)&c_addrlen);
        getpeername(client_sockfd, (struct sockaddr*) &client_info, (socklen_t*) &c_addrlen);
        printf("Client user %s:%d come in.\n", inet_ntoa(client_info.sin_addr), ntohs(client_info.sin_port));
        printf("Reached here");
        ClientList *c = newNode(client_sockfd, inet_ntoa(client_info.sin_addr));
//        addMap(c);

        pthread_t tid;
        if (pthread_create(&tid,NULL,client_handler, c) != 0)
        {
            perror("Create pthread error!\n");
            exit(EXIT_FAILURE);
        }


    }
        printf("you are out");

    close(server_sockfd);
    return 0 ;

}
