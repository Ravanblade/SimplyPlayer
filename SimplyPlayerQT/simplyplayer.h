#ifndef SIMPLYPLAYER_H
#define SIMPLYPLAYER_H

#include <QMainWindow>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QTimer>
#include <QMouseEvent>
#include <QFont>
#include <QFontDatabase>
#include <QVariant>
#include <QList>
#include <QGraphicsDropShadowEffect>
#include <windows.h>
#include <QtWinExtras/QtWinExtras>
#include "simplysettings.h"

#include "qtimerthread.h"
#include "simplytabbar.h"
#include "spectrumanalyzer.h"
#include "readers.h"
#include "simplyfunctions.h"
#include "simplydefinitions.h"
#include "simplyexpander.h"
#include "pictureflow.h"
#include "simplydockwindow.h"
#include "HTTP_Downloader.h"
#include "qyoutubeparser.h"
#include "qplaylist.h"
#include "addnewplaylist.h"
#include "simplygraphicpreloader.h"


namespace Ui {
    class SimplyWindow;
}

class SimplyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SimplyWindow(QWidget *parent = 0);
    ~SimplyWindow();
    SIMPLY_PLAYLIST AddFileToPlaylist(QString Path, QPlaylist *playlist, QString time = "", QString name = "");
    QString OneToTwo(int Org);
    QTimer *PlaylistTimer;
    QTimer *LabelTimer;
    QTimer *ResizeTimer;
    QTimer *SignalsTimer;
    QTimer *SpectrumTimer;
    QTimer *DownloadTimer;
    int Counter;
    int percent;
    SIMPLY_LABEL state;
    Ui::SimplyWindow *ui;
    void SetPLH(int h);
    void LoadPlayerSettings();
    void SavePlayerSettings();
    void SetupStyleSheets();
    bool IsSupportedFormat(QString format);
    SIMPLY_PLS LoadPLS(QString path, QPlaylist *playlist);
    SIMPLY_PLS SavePLS(QString path, QPlaylist *playlist);
    void addNewPlaylist(QString name, QString file_path);
    void DrawSpectrum();
    void LoadSkin();
    char *file;
    bool twice;
    void SavePlaylists();
    void CreateMirrorClock();
   // void OpenPlaylist();
    QPlaylist *currentPlaylist;
    QWidget *currentTab;
    void ParsePlaylists();
   // void on_Playlist_doubleClicked(QTreeWidgetItem * item);
    void SavePlaylist(QString playlist_name, QString playlist_file, int number);
    void AddEntryToPlaylistFile(QString playlist_name, QString playlist_file);
    QPalette TMPPal;
    SimplyTabBar *tabbar;
    QTab *newTab;
    void DeleteEntryToPlaylistFile(QString playlist_name);
    QPushButton *delegateDelete;
    QPushButton *delegateRepeat;
    QString ConvertWrzutaAddress(QString url);
    void PlayStop();
    PictureFlow *flow;
    SimplyDockWindow *playlistDock;
    SimplyDockWindow *albumInfo;
    //SimplyDockWindow *webDock;
    void initMenus();
    SimplyMediaPlayer *Player;
    void setupThumbButtons();
    QWinThumbnailToolBar *thumbbar;
    QWinThumbnailToolButton *playthumb;
    QWinThumbnailToolButton *stopthumb;
    void loadPlugins();


public slots:
//    void playFileSlot(QYouTubeParser parser);

    //void createEventTimer(FileDownloader *filedown, int event);


protected:
    void closeEvent(QCloseEvent * event);
    QRegion childRegion;
    void keyPressEvent(QKeyEvent *k);
    void paintEvent(QPaintEvent *event);
    void moveEvent (QMoveEvent * event);
    void showEvent(QShowEvent * event);
    void changeEvent( QEvent* e );
    void focusInEvent(QFocusEvent * event);

   // void moveEvent(QMoveEvent *event);


   private:
    QAbstractItemModel *model;
    QGraphicsPixmapItem *coveritem;
    QPoint dragPosition;
    QPixmap widgetMask;
    BYTE alpha;
    bool useSkin,useGlassEffect;
    QTimer *lower;
    LOWER_TYPE type;
    int last_volume;
    QFont playlist_font;
    int volume;
    int repeat_index;
    bool nextIndex;
    QColor highlightColor, styleHighlightColor;
    bool afterInit;
    bool isBBclicked; // bar button clicked
    QMenu mainMenu;
    float alphaPL,alphaAI;
    //QList<DOWNLOAD_FILE*> downloadList;
   // bool isHTTP(QString link);
   // QSignalMapper *signalMapper;
    QAction *playlista;
    QAction *albuminf;
    AddNewPlaylist *crpl;
    SimplyGraphicPreloader *loadedGraphic;
    int last_index;
    SimplySettings *settingsForm;
    QString youtubedl_path;
    QProcess *youtube_dl;
    int youtube_temp_index;
    short int connecting_dots;
    bool bufferEnabled;


private slots:
    void on_Balance_sliderReleased();
    void on_Volume_sliderReleased();
    void on_Volume_sliderPressed();
    void on_Balance_sliderPressed();
    void on_Backward_clicked();
    void on_Forward_clicked();
    void on_Stop_clicked();
    void on_Pause_clicked();
    void on_Play_clicked();
    void on_Balance_sliderMoved(int position);
    void on_Volume_sliderMoved(int position);
    void on_Open_clicked();
    void on_trackbar_sliderMoved(int position);
    void resizeEvent(QResizeEvent *e);
    void onClickTrackBar();
    void barButtonClick();
  //  void CreateNewPlaylist(QString name, QString path);
    void showPlaylistContextMenu(const QPoint& pos);
    void playlistDockContextMenu(const QPoint& pos);
    void albumInfoContextMenu(const QPoint& pos);
    void on_trackbar_sliderPressed();
    void removeItemFromPlaylist(int row);
    void onCloseTab(int index);
    void onDeleteItem();
    void setRepeat();
    void searchMusic();
    void checkPlaylistWidget();
    void checkAlbumInfoWidget();
    void on_findEdit_textChanged(const QString &arg1);
    void expandPlaylist();
    void on_tabWidget_currentChanged(int index);
    void closeMainWindow();
    void minMainWindow();
    void maxMainWindow();
    void closePlaylistWindow();
    void closeAlbumInfoWindow();
    void minPlaylistWindow();
    void maxPlaylistWindow();
    void doubleClickCover();
    void openSettings();
    void wrzutaPlay(QString data, int mode);
    void youtubePlay(QString data, int mode);
   // void showPageData(FILEDOWNLOAD_MAP *map);
    void on_textBrowser_textChanged();
    void deletePlaylistButton();
    void addNewPlaylistButton();
    void createNewPlaylist(QString name, QString author, QString coverpath);
    void editPlaylistButton();
    void reloadPlaylist(QString name, QString path, QPlaylist *playlist, bool isAct);
    void youtubedlProcessFinished(int errorcode);
    void youtubedlDownloadFinished(int itemindex);

protected slots:
    void PlaylistTimerEvent();
    void LabelTimerEvent();
    void ResizeTimerEvent();
    void SpectrumTimerEvent();
    void LowerTimerEvent();
    void doubleClicked(QTreeWidgetItem * item,int column);
    void SignalsTimerEvent();
    void resizePlaylistWindow();
    void movePlaylistWindow();
    void moveAlbumInfoWindow();
    void resizeAlbumInfoWindow();
    void downloadTimerEvent();





};





#endif // SIMPLYPLAYER_H




