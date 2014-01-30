// Author: Justin Fung

#ifndef File_H
  #define	File_H

#include "Time.h"
#include "permissions.h"

class File
{
  char *name;
  Time modTime;
  Permissions permissions;
  
protected:
  virtual ostream& write(ostream &os) const;
  
public:
  File(const char *name, const Permissions &perm = Permissions(00666, NULL));
  File(const File &rhs);
  virtual ~File();
  void changePerm(short perms);
  bool checkPerm(short perms, const char *user) const;
  void chown(const char *user);
  bool isFile() const;
  void print(bool val) const;
  void rename(const char *name);
  void showName() const;
  const char* getName() const;
  const char* getOwn() const;
  void touch();
  bool operator==(const File &rhs) const;
  bool operator<(const File &rhs) const;
  friend ostream& operator<<(ostream&os, const File &file);
  friend istream& operator>>(istream &is, const File &file);
  
}; // File class

#endif	// File_H