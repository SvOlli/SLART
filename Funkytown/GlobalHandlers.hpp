/**
 * GlobalHandlers.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef GLOBALHANDLERS_HPP
#define GLOBALHANDLERS_HPP GLOBALHANDLERS_HPP

class DownloadHandler;
class PostDownloadHandler;

extern DownloadHandler     *gpDownloadHandler;
extern PostDownloadHandler *gpPostDownloadHandlerHTML;
extern PostDownloadHandler *gpPostDownloadHandlerXML;
extern PostDownloadHandler *gpPostDownloadHandlerMP3;
extern PostDownloadHandler *gpPostDownloadHandlerFLV;

#endif
