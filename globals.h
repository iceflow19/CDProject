/* 
 * File:   globals.h
 * Author: Thomas Royko
 *
 * Created on February 28, 2016, 10:11 PM
 */

#ifndef GLOBALS_H
#define GLOBALS_H

//Sector Stuff
#define LOGICAL_SECTOR_SIZE     2048
#define PVD_SECTOR              16
#define TVD_SECTOR              17
#define LPATH_SECTOR            18
#define MPATH_SECTOR            19
#define ROOT_RECORD_SECTOR      20
#define PADDING_SECTOR          21

//Commandline parsing stuff
#define BLANK_FILE_IDENTIFIER   "            "
#define NO_SYSTEM_ID            "NO SYSTEM ID"
#define NO_VOLUME_ID            "NO VOLUME ID"
#define NO_VOLUME_SET_ID        "NO-VOLUME-SET-ID"
#define APP_IDENTIFIER          "CDProject"
#define DEFAULT_DATE            "2100010100000000\22"

//VD Stuff
#define PVD_TYPE                1
#define TVD_TYPE                255
#define VD_VERSION              1
#define VD_ID                   "CD001"
#define VOLUME_SPACE_SIZE       (uint64_t)0xAE000000000000AE
#define FILE_STRUCTURE_VERSION  1

#endif /* GLOBALS_H */

