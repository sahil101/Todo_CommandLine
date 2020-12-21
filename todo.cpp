#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
using namespace std;

//Reading the contents of the file
vector<string> read_tasks(string filename)
{
    fstream file;
    //Opening the file in read mode
    file.open(filename, ios::in);
    //To store the tasks
    vector<string> vec;
    if (file.is_open())
    {
        string tp;
        while (getline(file, tp))
            vec.push_back(tp);
        file.close();
    }
    file.close();
    return vec;
}
//todo help
void describe_usage()
{
    cout << "Usage :-\n";
    cout << "$ ./todo add \"todo item\" \t# Add a new todo\n";
    cout << "$ ./todo ls   \t\t\t# Show remaining todos\n";
    cout << "$ ./todo del NUMBER \t\t# Delete a todo\n";
    cout << "$ ./todo done NUMBER\t\t# Complete a todo\n";
    cout << "$ ./todo help   \t\t# Show usage\n";
    cout << "$ ./todo report   \t\t# Statistics";
}
//Adding Task
void add_task(string task, string filename, bool flag)
{
    ofstream file;
    file.open(filename, ios::app);
    file << task << "\n";
    file.close();
    if (flag)
        cout << "Added todo: "
             << "\"" << task << "\"";
}
//Listing all the tasks pending
void listing_tasks()
{
    vector<string> vec = read_tasks("todo.txt");
    //Display in reverse order
    int n = vec.size();
    if (n == 0)
    {
        cout << "There are no pending todos!";
        return;
    }
    string display = "";
    for (int i = n - 1; i >= 1; i--)
        display = display + "[" + to_string(i + 1) + "] " + vec[i] + "\n";
    display = display + "[" + "1" + "] " + vec[0];
    cout << display;
}
//Deleting a todo task from the file
string del_task(int num, bool flag)
{
    vector<string> vec = read_tasks("todo.txt");
    //Checking if the number is valid or not
    int n = vec.size();
    if (num >= 1 && num <= n)
    {
        //Clearing entire file
        ofstream truncate;
        truncate.open("todo.txt", ios::trunc);
        truncate.close();
        //appending all the tasks except the number which is mentioned
        ofstream append;
        append.open("todo.txt", ios::app);
        string deleted_task;
        for (int i = 0; i < n; i++)
        {
            if (i + 1 != num)
                append << vec[i] << "\n";
            else
                deleted_task = vec[i];
        }
        append.close();
        if (flag)
            cout << "Deleted todo #" << num;
        return deleted_task;
    }
    //Error message if the number is invalid
    else
    {
        cout << "Error: todo #" << num << " does not exist. Nothing deleted.";
    }
}
//Append Done task to done file
void done_task(int num)
{
    vector<string> vec = read_tasks("todo.txt");
    int n = vec.size();
    if (num >= 1 && num <= n)
    {

        time_t t = time(NULL);
        tm *timePtr = localtime(&t);
        string completed_task = del_task(num, false);
        string date_format = to_string(timePtr->tm_year + 1900) + "-" + to_string(timePtr->tm_mon + 1) + "-" + to_string(timePtr->tm_mday) + " ";
        completed_task = date_format + completed_task;
        add_task(completed_task, "done.txt", false);
        cout << "Marked todo #" << num << " as done.";
    }
    else
    {
        cout << "Error: todo #" << num << " does not exist.";
    }
}
//Statistics of the todo list
void generate_report()
{
    vector<string> pending_tasks = read_tasks("todo.txt");
    vector<string> completed_tasks = read_tasks("done.txt");
    time_t t = time(NULL);
    tm *timePtr = localtime(&t);
    cout << timePtr->tm_year + 1900 << "-" << timePtr->tm_mon + 1 << "-" << timePtr->tm_mday << " ";
    cout << "Pending : " << pending_tasks.size() << " "
         << "Completed : " << completed_tasks.size();
}
int main(int argc, char *argv[])
{
    //Checking for commands that only have two values
    if (argc == 1)
    {
        describe_usage();
        return 0;
    }
    string str = argv[1];
    if (str == "help")
        describe_usage();
    if (str == "add")
    {
        if (argc == 2)
        {
            cout << "Error: Missing todo string. Nothing added!";
            return 0;
        }
        string task = argv[2];
        add_task(task, "todo.txt", true);
    }
    if (str == "ls")
        listing_tasks();
    if (str == "del")
    {
        if (argc == 2)
        {
            cout << "Error: Missing NUMBER for deleting todo.";
            return 0;
        }
        string str = argv[2];
        int num = stoi(str);
        del_task(num, true);
    }
    if (str == "done")
    {
        if (argc == 2)
        {
            cout << "Error: Missing NUMBER for marking todo as done.";
            return 0;
        }
        string str = argv[2];
        int num = stoi(str);
        done_task(num);
    }
    if (str == "report")
        generate_report();
    return 0;
}
