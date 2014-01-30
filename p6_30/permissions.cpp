// Author: Justin Fung

#include <iostream>
#include <iomanip>
#include <cstring>
#include "permissions.h"

using namespace std;

short Permissions::umask = 022;

Permissions::Permissions(short originalPermissions, const char *owne)
{
  value = originalPermissions & ~umask;
  type = originalPermissions / 1000;
  
  if(owne)
  {
    owner = new char[strlen(owne) + 1];
    strcpy(owner, owne);
  } // if owne not NULL
  else // owne == NULL
    owner = NULL;
} // Permissions()


Permissions::Permissions(const Permissions &rhs) 
  : value(rhs.value), type(rhs.type)
{
  if(rhs.owner)
  {
    owner = new char[strlen(rhs.owner) + 1];
    strcpy(owner, rhs.owner);
  } // if rhs has non-NULL owner
  else // rhs.owner == NULL
    owner = NULL;
}  // Permissions()


Permissions::~Permissions()
{
  if(owner)
    delete owner;
}  // ~Permissions()


bool Permissions::check(short permissions, const char *user) const
{
  if(strcmp(user, owner)) // different user from owner
    return ((value & 7) & permissions)  == permissions;

  return (((value >> 6) & 7) & permissions) == permissions;
}  // check()


void Permissions::chmod(short permissions)
{
  value = permissions;
} // chmod()


const char* Permissions::getOwner() const
{
  return owner;
} // getOwner()


short Permissions::getType() const
{
  return type;
} // getType()


void Permissions::print() const
{
  if(type == 1)
    cout << "d";
  else
    cout << "-";
  
  for(int i = 6; i >= 0; i -= 3)
  {
    short octal = (value >> i) & 7;

    if(octal & 4)
      cout << "r";
    else // octal & 4 == 0
      cout << "-";

    if(octal & 2)
      cout << "w";
    else  // 2 bit cleared
      cout << "-";

    if(octal & 1)
      cout << "x";
    else  // 1 bit clear
      cout << "-";
  } // for each set of triplets

  cout << ' ' << setw(10) << owner << ' ';
} // print()


void Permissions::readUmask(istream &is)
{
  is >> oct >> umask;
  is.ignore();
} // readUmask()


void Permissions::setOwner(const char *owne)
{
  if(owner)
    delete owner;

  if(owne)
  {
    owner = new char [strlen(owne) + 1];
    strcpy(owner, owne);
  } // if owne is not NULL
  else // owne == NULL
    owner = NULL;
} // setOwner()


void Permissions::setUmask( int argCount, const char *arguments[])
{
  short octal = 0;

  if(argCount != 2)
  {
    cout << "usage: umask octal\n";
    return;
  } // if not 2 arguments

  for(int i = 0; arguments[1][i]; i++)
    if(!isdigit(arguments[1][i]) || arguments[1][i] == '9' ||
       arguments[1][i] == '8')
    {
      cout << "usage: umask octal\n";
      return;
    } // if not octal digit
    else
      octal = octal * 8 + arguments[1][i] - '0';


  if(octal < 0 || octal > 0777)
  {
    cout << "umask: octal must be between 0 and 777\n";
    return;
  } // if octal invalid

  umask = octal;
} // setUmask()


void Permissions::writeUmask(ostream & os)
{
  os << oct << umask << endl;
} // writeUmask()


ostream& operator<< (ostream & os, const Permissions &permissions)
{
  os << oct << permissions.value << ' ' << permissions.type << ' ' << 
    permissions.owner << ' ';
  return os;
} // operator<<


istream& operator>> (istream & is, Permissions &permissions)
{
  char s[256];
  
  is >> oct >> permissions.value;
  is.ignore();  // eat space char
  is >> permissions.type;
  is.ignore();
  is >> s;
  is.ignore();
  permissions.owner = new char[strlen(s) + 1];
  strcpy(permissions.owner, s);
  
  return is;
} // operator>>