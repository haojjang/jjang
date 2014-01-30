// Author: Justin Fung

#ifndef PERMISSIONS_H
	#define PERMISSIONS_H

#include <iostream>

using namespace std;

class Permissions
{
  short value;
  short type;
  static short umask;
  char *owner;
public:
  Permissions(short originalPermissions, const char *owne);
  Permissions(const Permissions &p);
  ~Permissions();
  bool check(short permissions, const char *user) const;
  void chmod(short permissions);
  const char* getOwner() const;
  short getType() const;
  void print() const;
  static void readUmask(istream &is);
  void setOwner(const char *owne);
  static void setUmask( int argCount, const char *arguments[]);
  static void writeUmask(ostream &os);
  friend ostream& operator<< (ostream & os, const Permissions &permissions);
  friend istream& operator>> (istream & is, Permissions &permissions);
};  // class Permissions

#endif