// Author: Justin Fung

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "directory.h"
#include "Time.h"
#include "permissions.h"

using namespace std;

Directory::Directory(const char *nam, Directory *paren, const char* owner)
  : File(nam, Permissions(01777, owner)), subDirectoryCount(0), 
    permissions(01777, owner), parent(paren)
{
  name = new char[strlen(nam) + 1];
  strcpy(name, nam);
} // Directory()


Directory::Directory(const Directory &rhs) 
  : File(rhs), subDirectoryCount(rhs.subDirectoryCount),
  permissions(rhs.permissions), parent(rhs.parent)
{
  name = new char[strlen(rhs.name) + 1];
  strcpy(name, rhs.name);

  for(int i = 0; i < subDirectoryCount; i++)
  {
    if(rhs.subDirectories[i]->isFile())
      subDirectories += new File(*(rhs.subDirectories[i]));
    else
    {
    Directory *temp = new Directory(*((Directory*) rhs.subDirectories[i]));
    temp->parent = this;
    subDirectories += temp;
    } // if file, add file, else add directory to linkedlist
  }  // for each subdirectory
} // Directory copy constructor


Directory* Directory::cd(int argCount, const char *arguments[],
                         const char *user)
{
  if(argCount != 2)
  {
    cout << "usage: cd directoryName\n";
    return this;
  } // if incorrect number of arguments

  if(strcmp(arguments[1], "..") == 0)
  {
    if(parent)
    {
      if(! parent->checkPerm(5, user))
      {
        cout << "cd: Permission denied.\n";
        return this;
      } // if incorrect permissions
      return parent;
    }
    else // root directory
      return this;
  } // if cd ..

  for(int i = 0; i < subDirectoryCount; i++)
    if(Directory(arguments[1]) == *subDirectories[i])
    {
      if(! subDirectories[i]->checkPerm(5, user))
      {
        cout << "cd: Permission denied.\n";
        return this;
      } // if incorrect permissions for this subdirectory

      if(subDirectories[i]->isFile())
      {
        cout << arguments[1] << "cd: Permission denied.\n";
        return this;
      }
      else
        return (Directory*)subDirectories[i];
    } // if found subdirectory match

  cout << arguments[1] << ": no such file or directory.\n";
  return this;
} // cd()


void Directory::chmod(int argCount, const char *arguments[], const char *user)
{
  int i, j, value = 0;

  if(argCount < 3)
  {
    cout << "chmod: too few arguments\n";
    return;
  } // if incorrect argument count.

  for(i = 0; arguments[1][i]; i++)
    if(! isdigit(arguments[1][i]) || arguments[1][i] == '9'
      || arguments[1][i] == '8')
    {
      cout << "chmod: invalid mode string: '" << arguments[1] << "'\n";
      return;
    } // if invalid mode
    else // valid digit
      value = 8 * value + arguments[1][i] - '0';

  if(value < 0 || value > 0777)
  {
    cout << "chmod: invalid mode string: '" << arguments[1] << "'\n";
    return;
  } // if invalid value

  for(i = 2; i < argCount; i++)
  {
    for(j = 0; j < subDirectoryCount; j++)
      if(*subDirectories[j] == Directory(arguments[i]))
      {
        if(strcmp(user, "root") && 
           strcmp(user, subDirectories[j]->getOwn()) != 0 
           && ! subDirectories[j]->checkPerm(2, user))
        {
          cout << "chmod: Permission denied.\n";
          return;
        } // if invalid permissions for subdirectory

        subDirectories[j]->changePerm(value);
        break;
      } // if found subdirectory match

    if(j == subDirectoryCount)
      cout << "chmod: failed to get attributes of '" << arguments[i]
        << "': No such file or directory\n";
  } // for each file
} // chmod()


void Directory::chownCMD( int argCount, const char *arguments[],
                          const char *user)
{
  int i, j;

  if(argCount < 3)
  {
    cout << "chown: too few arguments\n";
    return;
  } // if incorrect argument count.


  for(i = 2; i < argCount; i++)
  {
    for(j = 0; j < subDirectoryCount; j++)
      if(*subDirectories[j] == Directory(arguments[i]))
      {
        if(strcmp("root", user))
        {
          cout << "chown: Permission denied.\n";
          return;
        } // if invalid permissions for subdirectory

        subDirectories[j]->chown(arguments[1]);
        break;
      } // if found subdirectory match

    if(j == subDirectoryCount)
      cout << "chown: failed to get attributes of '" << arguments[i]
        << "': No such file or directory\n";
  } // for each file in arguments

} // chown()


void Directory::cp( int argCount, const char *arguments[], const char *user)
{
  int i, j;

  if(argCount == 1)
  {
    cout << "cp: missing file arguments\n";
    return;
  } // if incorrect argument count.

  if(argCount == 2)
  {
    cout << "cp: missing destination file\n";
    return;
  } // if incorrect argument count.

  if(argCount > 3)
  {
    cout << "cp: too many arguments\n";
    return;
  } // if incorrect argument count.

  for(i = 0; i < subDirectoryCount; i++)
    if(*subDirectories[i] == Directory(arguments[1]))
      break;

  if(i == subDirectoryCount)
  {
    cout << "cp: cannot stat '" << arguments[1]
      << "': No such file or directory\n";
    return;
  } // if didn't find file

  if(!strcmp(arguments[1], arguments[2]))
  {
    cout << "cp: '" << arguments[1] << "' and '" << arguments[2]
      << "' are the same file\n";
    return;
  } // if same names

  for(j = 0; j < subDirectoryCount; j++)
    if(*subDirectories[j] == Directory(arguments[2]))
    {
      cout << "cp: omitting '" << arguments[2] << "'\n";
      return;
    } // if destination already exists
  
  if(! subDirectories[i]->checkPerm(4, user))
  {
    cout << "cp : Permission denied.\n";
    return;
  } // if permissions not OK

  if(subDirectories[i]->isFile())
  {
    File *temp = new File(*(subDirectories[i]));
    temp->rename(arguments[2]);
    temp->chown(user);
    subDirectories += temp;
    subDirectoryCount++;
  } // if object is a file
  else  // isFile() returns 0
  {
    Directory *temp = new Directory(*((Directory*)subDirectories[i]));
    temp->rename(arguments[2]);
    temp->chown(user);
    temp->permissions.setOwner(user);
    subDirectories += temp;
    subDirectoryCount++;
  } // if object is a directory
} // cp()


void Directory::ls(int argCount, const char *arguments[],
                   const char *user)const
{
  if(argCount > 2 || (argCount == 2 && strcmp(arguments[1], "-l")))
  {
    cout << "usage: ls [-l]\n";
    return;
  } // if incorrect number of arguments

  if(subDirectoryCount == 0)
    return;

  if(! this->checkPerm(4, user))
  {
    cout << "ls: Permission denied.\n";
    return;
  } // if invalid permissions
  
  if(argCount == 2) // must be ls -l
  {
    for(int i = 0; i < subDirectoryCount; i++)
    {
      subDirectories[i]->print(1);
      cout << " ";
      subDirectories[i]->print(0);
      cout << ' ';
      subDirectories[i]->showName();
      cout << endl;
    }  // for each subdirectory
  } // if ls -l
  else // simple ls
  {
    for(int i = 0; i < subDirectoryCount; i++)
    {
      subDirectories[i]->showName();
      cout << " ";
    } // for each subdirectory

    cout << endl;
  } // else simple ls
} // ls()


void Directory::mkdir(int argCount, const char *arguments[], const char *user)
{
  if(argCount != 2)
  {
    cout << "usage: mkdir directory_name\n";
    return;
  } // if wrong number of arguments

  for(int i = 0; i < subDirectoryCount; i++)
    if(strcmp(arguments[1], subDirectories[i]->getName()) == 0)
    {
      cout << "mkdir: cannot create directory '" << arguments[1]
        << "': File exists\n";
      return;
    } // if found file with same name

  if(! this->checkPerm(6, user))
  {
    cout << "mkdir: Permission denied.\n";
    return;
  } // if don't have permissions 

  subDirectories += new Directory(arguments[1], this, user);
  subDirectoryCount++;
  modificationTime.update();

} // mkdir()


void Directory::touch(int argCount, const char* arguments[], const char* user)
{
  int count;
  
  if(argCount < 2)
  {
    cout << "touch: too few arguments\n";
    return;
  } // if wrong number of arguments
  
  for(int i = 1; i < argCount; i++)
  {
    count = 0;
    for(int j = 0; j < subDirectoryCount; j++)
      if(strcmp(arguments[i], subDirectories[j]->getName()) == 0)
      {
        if(! subDirectories[j]->checkPerm(2, user))
          cout << "touch: Permission denied.\n";
        else
          subDirectories[j]->touch();
      } // if found file with same name
      else  // file does not have the same name
        count++;

    if(count == subDirectoryCount)  // if file name is unique
    {
      if(! this->checkPerm(6, user))
        cout << "touch: Permission denied.\n";
      else  // permissions granted
      {
        subDirectories += new File(arguments[i], Permissions(00666, user));
        subDirectoryCount++;
        modificationTime.update();
      } // if user has required permissions
    } // if file name is unique
  } // scan arguments

} // touch()


void Directory::showPath() const
{
  if(parent)
  {
    parent->showPath();
    cout << this->getName() << "/";
  } // if not root
  else
    cout << this->getName();
} // showPath()


ostream& operator<< (ostream &os, const Directory &directory)
{
  os << directory.name << ' ' <<  directory.modificationTime <<
    directory.permissions << directory.subDirectoryCount << endl;

  for(int i = 0; i < directory.subDirectoryCount; i++)
  {
    if(directory.subDirectories[i]->isFile())
    {
      os << 0 << ' ';  // setting marker to indicate type file
      os << *(directory.subDirectories[i]);
    } // if object is a file
    else
    {
      os << 1 << ' ';  // setting marker to indicate type directory
      os << *((Directory*)directory.subDirectories[i]);
    } // if object is a directory
  } // for each subdirectory object

  return os;
}  // operator<<


istream& operator>> (istream &is, Directory &directory)
{
  bool type;
  char temp[80];
  
  is >> type;
  is.ignore();   // eat space char
  is >> temp;
 
  if(directory.name)
    delete [] directory.name;

  directory.name = new char[strlen(temp) + 1];
  strcpy(directory.name, temp);
  
  is.ignore();  // eat space char
  is >> directory.modificationTime >> directory.permissions
    >> directory.subDirectoryCount;

/*  for(int i = 0; i < directory.subDirectoryCount; i++)
  {
    if(type)
    {
      Directory *subDirectory = new Directory("Dummy");
      is >> *subDirectory;
      subDirectory->parent = &directory;
      directory.subDirectories += subDirectory;
    }
    else
    {
      File *file = new File("Dummy", Permissions(00666, NULL));
      is >> *file;
      directory.subDirectories += file;
    }
  } // for each subdirectory
*/
  return is;
}  // operator<<