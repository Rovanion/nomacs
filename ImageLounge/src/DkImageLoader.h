/*******************************************************************************************************
 DkImageLoader.h
 Created on:	21.04.2011
 
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2013 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2013 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2013 Florian Kleber <florian@nomacs.org>

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************************************/

#pragma once

#pragma warning(push, 0)	// no warnings from includes - begin
//#include <QColor>
#include <QTimer>
//#include <QThread>
//#include <QMutex>
//#include <QStringList>
#include <QImage>
#pragma warning(pop)	// no warnings from includes - end

#ifndef DllExport
#ifdef DK_DLL_EXPORT
#define DllExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllExport Q_DECL_IMPORT
#else
#define DllExport
#endif
#endif

// my classes
#include "DkImageContainer.h"

#ifdef Q_WS_X11
	typedef  unsigned char byte;
#endif

// Qt defines
class QFileSystemWatcher;
class QUrl;

namespace nmc {

/**
 * This class is a basic image loader class.
 * It takes care of the file watches for the current folder,
 * holds the currently displayed image,
 * calls the load routines
 * and saves the image or the image metadata.
 **/ 
class DllExport DkImageLoader : public QObject {
	Q_OBJECT

public:

	DkImageLoader(const QString& filePath = QString());
	virtual ~DkImageLoader();

	static QStringList getFoldersRecursive(QDir dir);
	QFileInfoList updateSubFolders(QDir rootDir);
	QFileInfoList getFilteredFileInfoList(const QString& dirPath, QStringList ignoreKeywords = QStringList(), QStringList keywords = QStringList(), QStringList folderKeywords = QStringList());

	void rotateImage(double angle);
	QSharedPointer<DkImageContainerT> getCurrentImage() const;
	QSharedPointer<DkImageContainerT> getLastImage() const;
	QString filePath() const;
	QStringList getFileNames();
	QVector<QSharedPointer<DkImageContainerT> > getImages();
	void setImages(QVector<QSharedPointer<DkImageContainerT> > images);
	void firstFile();
	void lastFile();
	void clearPath();
	QString getDirPath() const;
	QDir getSaveDir() const;
	void loadLastDir();
	void setDir(const QString& dir);
	void setSaveDir(const QString& dir);
	QSharedPointer<DkImageContainerT> setImage(const QImage& img, const QString& editFilePath = QString());
	QSharedPointer<DkImageContainerT> setImage(QSharedPointer<DkImageContainerT> img);
	bool hasFile() const;
	bool hasMovie();
	QString fileName();
	QSharedPointer<DkImageContainerT> getSkippedImage(int skipIdx, bool searchFile = true, bool recursive = false);
	void sort();
	QSharedPointer<DkImageContainerT> findOrCreateFile(const QString& filePath) const;
	QSharedPointer<DkImageContainerT> findFile(const QString& filePath) const;
	int findFileIdx(const QString& filePath, const QVector<QSharedPointer<DkImageContainerT> >& images) const;
	void setCurrentImage(QSharedPointer<DkImageContainerT> newImg);
#ifdef WITH_QUAZIP
	bool loadZipArchive(const QString& zipPath);
#endif
	
	void deactivate();
	void activate(bool isActive = true);
	bool hasImage() const;
	bool isEdited() const;
	int numFiles() const;
	QImage getImage();
	bool dirtyTiff();

	static bool restoreFile(const QString &filePath);

signals:
	void folderFiltersChanged(QStringList filters) const;
	void updateImageSignal() const;
	void updateInfoSignalDelayed(QString msg, bool start = false, int timeDelayed = 700) const;
	void updateSpinnerSignalDelayed(bool start = false, int timeDelayed = 700) const;
	void updateFileSignal(const QString& filePath, QSize s = QSize(), bool edited = false, QString attr = QString()) const;
	void fileNotLoadedSignal(const QString& filePath) const;
	void setPlayer(bool play) const;
	void updateFileWatcherSignal(const QString& filePath) const;

	// new signals
	void imageUpdatedSignal(QSharedPointer<DkImageContainerT> image);
	void imageUpdatedSignal(int idx);	// folder scrollbar needs that
	void imageLoadedSignal(QSharedPointer<DkImageContainerT> image, bool loaded = true);
	void showInfoSignal(QString msg, int time = 3000, int position = 0);
	void updateDirSignal(QVector<QSharedPointer<DkImageContainerT> > images);
	void imageHasGPSSignal(bool hasGPS);

public slots:
	void changeFile(int skipIdx);
	void directoryChanged(const QString& path = QString());
	void saveFileWeb(QImage saveImg);
	void saveUserFileAs(QImage saveImg, bool silent);
	void saveFile(const QString& filename, const QImage& saveImg = QImage(), const QString& fileFilter = "", int compression = -1, bool threaded = true);
	void load(QSharedPointer<DkImageContainerT> image = QSharedPointer<DkImageContainerT>());
	void load(const QString& filePath);
	void downloadFile(const QUrl& url);
	bool deleteFile();
	QString saveTempFile(const QImage& img, const QString& name = "img", const QString& fileExt = ".png", bool force = false, bool threaded = true);
	void setFolderFilter(const QString& filter);
	void setFolderFilters(const QStringList& filters);
	QStringList getFolderFilters();
	bool loadDir(const QString& newDirPath, bool scanRecursive = true);
	void errorDialog(const QString& msg) const;
	void loadFileAt(int idx);

	// new slots
	void imageLoaded(bool loaded = false);
	void imageSaved(const QString& file, bool saved = true);
	void imagesSorted();
	bool unloadFile();
	void reloadImage();

protected:

	QStringList mIgnoreKeywords;
	QStringList mKeywords;
	QStringList mFolderKeywords;		// are deleted if a new folder is opened

	QTimer delayedUpdateTimer;
	bool timerBlockedUpdate;
	QString mCurrentDir;
	QString mSaveDir = "";
	QFileSystemWatcher* dirWatcher;
	QStringList subFolders;
	QVector<QSharedPointer<DkImageContainerT > > mImages;
	QSharedPointer<DkImageContainerT > currentImage;
	QSharedPointer<DkImageContainerT > lastImageLoaded;
	bool folderUpdated;
	int tmpFileIdx;
	bool sortingImages;
	bool sortingIsDirty;
	QFutureWatcher<QVector<QSharedPointer<DkImageContainerT > > > createImageWatcher;

	// functions
	void updateCacher(QSharedPointer<DkImageContainerT> imgC);
	int getNextFolderIdx(int folderIdx);
	int getPrevFolderIdx(int folderIdx);
	void updateHistory();
	void sendFileSignal();
	QString getTitleAttributeString();
	void sortImagesThreaded(QVector<QSharedPointer<DkImageContainerT > > images);
	void createImages(const QFileInfoList& files, bool sort = true);
	QVector<QSharedPointer<DkImageContainerT > > sortImages(QVector<QSharedPointer<DkImageContainerT > > images) const;
};

// deprecated
class DkColorLoader : public QThread {
	Q_OBJECT

public:
	DkColorLoader(QVector<QSharedPointer<DkImageContainerT> > images);
	~DkColorLoader() {};

	void stop();
	void run();

	const QVector<QColor>& getColors() const;
	const QVector<int>& getIndexes() const;
	int maxFiles() const;
	QString getFilename(int idx) const;

signals:
	void updateSignal(const QVector<QColor>& cols, const QVector<int>& indexes);

protected:
	void init();
	void loadThumbs();
	void loadColor(int fileIdx);
	QColor computeColor(QImage& thumb);

	QVector<QSharedPointer<DkImageContainerT> > images;
	QVector<QColor> cols;
	QVector<int> indexes;
	bool isActive;
	bool paused;
	QMutex mutex;
	int maxThumbs;
};

};
