// Author: Justin Fung

#include <iostream>
#include <cstring>
#include "file.h"

using namespace std;

File::File(const char *nam, const Permissions &perm)
  : permissions(perm)
{
  name = new char[strlen(nam) + 1];
  strcpy(name, nam);
} // File()


File::File(const File &rhs)
  : permissions(rhs.permissions)
{
  name = new char[strlen(rhs.name) + 1];
  strcpy(name, rhs.name);
} // File copy constructor


File::~File()
{
  if(name)
    delete name;
} // ~File()


void File::changePerm(short perms)
{
  permissions.chmod(perms);
} // changePerm()


bool File::checkPerm(short perms, const char* user) const
{
  return permissions.check(perms, user);
} // checkPerm


void File::chown(const char* user)
{
  permissions.setOwner(user);
} // chown()


bool File::isFile() const
{
  if(permissions.getType() == 1)  // type is a directory
    return 0;
  else                            // type is a file
    return 1;
} // isFile()


void File::print(bool val) const
{
  if(val)                   // print permissions
    permissions.print();
  else                      // print time
    modTime.print();
} // print()


void File::rename(const char *nam)
{
  if(name)
    delete name;

  if(nam)
  {
    name = new char [strlen(nam) + 1];
    strcpy(name, nam);
  } // if nam is not NULL
  else // nam == NULL
    name = NULL;
} // rename()


void File::showName() const
{
  cout << name;
} // showName()


const char* File::getName() const
{
  return name;
} // getName()


const char* File::getOwn() const
{
  return permissions.getOwner();
} // getOwn()


void File::touch()
{
  modTime.update();
} // touch()


ostream& File::write(ostream& os) const
{
  os << name << ' ' << modTime << permissions << endl;
  
  return os;
} // write()


bool File::operator== (const File &rhs) const
{
  return !strcmp(name, rhs.name);
} // operator==


bool File::operator<(const File &rhs) const
{
  return strcmp(name, rhs.name) < 0;
} // operator<


ostream &operator<<(ostream &os, const File &file)
{
  file.write(os);
  return os;
} // operator<<

/*
istream& operator>>(istream &is, const File &file)
{
  char temp[80];
  
  is >> temp;
 
  if(file.name)
    delete [] file.name;

  file.name = new char[strlen(temp) + 1];
  strcpy(file.name, temp);
  is.ignore();  // eat space char
  is >> file.modTime >> file.permissions;
  
  return is;
} // operator>>
*/