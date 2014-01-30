// Author: Justin Fung

#ifndef DIRECTORY_H
	#define DIRECTORY_H

#include "permissions.h"
#include "Time.h"
#include "linkedlist.h"
#include "file.h"

class Directory: public File
{
  char *name;
  LinkedList<File> subDirectories;
  Time modificationTime;
  int subDirectoryCount;
  Permissions permissions;
  Directory *parent;
public:
  Directory(const char *name, Directory *parent = NULL,
            const char *owner = NULL);
  Directory(const Directory &rhs);
  Directory* cd(int argCount, const char *arguments[], const char *user);
  void chmod( int argCount, const char *arguments[], const char *user);
  void chownCMD( int argCount, const char *arguments[], const char *user);

  void cp( int argCount, const char *arguments[], const char *user);
  void ls(int argCount, const char *arguments[], const char *user) const;
  void mkdir(int argCount, const char *arguments[], const char *user);
  void touch(int argCount, const char *arguments[], const char *user);
  void showPath() const;
  friend ostream& operator<< (ostream &os, const Directory &directory);
  friend istream& operator>> (istream &is, Directory &directory);

}; // class Directory

#endif