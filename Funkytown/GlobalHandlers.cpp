/**
 * GlobalHandlers.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "GlobalHandlers.hpp"

#include "DownloadHandler.hpp"
#include "PostDownloadHandlerHTML.hpp"
#include "PostDownloadHandlerXML.hpp"
#include "PostDownloadHandlerMP3.hpp"
#include "PostDownloadHandlerFLV.hpp"

DownloadHandler     *gpDownloadHandler = 0;
PostDownloadHandler *gpPostDownloadHandlerHTML = 0;
PostDownloadHandler *gpPostDownloadHandlerXML = 0;
PostDownloadHandler *gpPostDownloadHandlerMP3 = 0;
PostDownloadHandler *gpPostDownloadHandlerFLV = 0;
