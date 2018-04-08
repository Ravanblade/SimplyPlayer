#include "simplyplayer.h"
#include "ui_simplyplayer.h"
#include "qskinobject.h"
#include "winuser.h"
#include "qskinobject.h"
#include "simplyhint.h"
#include "qtwin.h"
#include "qtab.h"
#include "loadingscreen.h"
#include "HTTP_Downloader.h"
#include "addlink.h"
#include "simplyhotkeymanager.h"
#include "websearch.h"

//extern Player *Player;
SimplyWindow *SimplyWindow;
extern WrzutaData *WrzutaD;

//-----------------------------------------------------------
int GlobalIndex = 0;
bool afterInit = false;
QSkinObject *Player_Skin;
QSkinObject *playlistSkin;
QSkinObject *albuminfoSkin;
QSkinObject *newplaylistSkin;
QSkinObject *settingsSkin;
QSkinObject *searchSkin;
SimplyHint *Simply_Hint;
WebSearch *src;

SimplyWindow::SimplyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimplyWindow)
{
    afterInit = false;
    ui->setupUi(this);
    Player = new SimplyMediaPlayer();
   // Player->Init();
    ui->Spectrum->setPlayer(Player);
    tabbar = new SimplyTabBar();
    tabbar->setDocumentMode(true);
    tabbar->setDrawBase(false);
    LoadPlayerSettings();
    ui->tabWidget->setTabBar(tabbar);
    tabbar->setParentWidget(ui->tabWidget);
    tabbar->setShape(QTabBar::RoundedNorth);
    tabbar->setMovable(true);
    //QThread *label_thread = new QThread();
    tabbar->setTabsClosable(true);

    this->setFocusPolicy(Qt::StrongFocus);
    connect(tabbar,SIGNAL(tabCloseRequested(int)),this,SLOT(onCloseTab(int)));

    nextIndex = false;

    twice = true;

/*    if(useGlassEffect)
    {   
        if (QtWin::isCompositionEnabled())
        {
            QtWin::extendFrameIntoClientArea(this);
            this->setContentsMargins(4, 4, 4, 4);

        }
    }
*/
    loadedGraphic = new SimplyGraphicPreloader();



    QList<QWidget *> widget_list;
    playlistDock = new SimplyDockWindow();
    albumInfo = new SimplyDockWindow();
  //  webDock = new SimplyDockWindow();

    widget_list << this << albumInfo;// << webDock;
    playlistDock->setDockWidgets(widget_list);

    QList<QWidget *> widget_list2;
    widget_list2 << this << playlistDock;// << webDock;
    albumInfo->setDockWidgets(widget_list2);


    /*QList<QWidget *> widget_list3;
    widget_list2 << this << playlistDock;// << webDock;
    webDock->setDockWidgets(widget_list2);
*/

    this->hide();
    albumInfo->hide();
    playlistDock->hide();

    playlistDock->setAcceptDrops(true);
    playlistDock->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("All.qss")));
    ui->tabWidget->setParent(playlistDock);
    ui->statusBar->setParent(playlistDock);
    ui->findEdit->setParent(playlistDock);
    playlistDock->resize(this->width(),700);


    QPushButton *addNewPL = new QPushButton();
    QPushButton *delPL = new QPushButton();
    QPushButton *importPL = new QPushButton();
    QPushButton *changePL = new QPushButton();

    addNewPL->setMinimumSize(5,5);
    addNewPL->setGeometry(11,11,5,5);

    delPL->setMinimumSize(5,5);
    delPL->setGeometry(21,21,5,5);

    importPL->setMinimumSize(5,5);
    importPL->setGeometry(31,31,5,5);

    changePL->setMinimumSize(5,5);
    changePL->setGeometry(41,41,5,5);

    connect(addNewPL, SIGNAL(clicked()), this, SLOT(addNewPlaylistButton()));
    connect(delPL, SIGNAL(clicked()), this, SLOT(deletePlaylistButton()));
    connect(importPL, SIGNAL(clicked()), this, SLOT(onDeleteItem()));
    connect(changePL, SIGNAL(clicked()), this, SLOT(editPlaylistButton()));

    addNewPL->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("addNewPlaylistButton.qss")));
    delPL->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("DeletePlaylistButton.qss")));
    importPL->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("ImportPlaylistButton.qss")));
    changePL->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("ChangePlaylistButton.qss")));

    ui->statusBar->addPermanentWidget(addNewPL,0);
    ui->statusBar->addPermanentWidget(delPL,0);
    ui->statusBar->addPermanentWidget(importPL,0);
    ui->statusBar->addPermanentWidget(changePL,0);


    ui->statusBar->addWidget(ui->findEdit);

    connect(playlistDock,SIGNAL(resizing()),this,SLOT(resizePlaylistWindow()));
    connect(playlistDock,SIGNAL(dockDown()),this,SLOT(movePlaylistWindow()));


    albumInfo->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("All.qss")));
    ui->graphicsView->setParent(albumInfo);
    ui->textBrowser->setParent(albumInfo);

    connect(albumInfo,SIGNAL(resizing()),this,SLOT(resizeAlbumInfoWindow()));
    connect(albumInfo,SIGNAL(dockDown()),this,SLOT(moveAlbumInfoWindow()));
    albumInfo->resize(150,100);
    ui->graphicsView->resize(150,100);
    ui->graphicsView->move(5,26);
    ui->textBrowser->resize(50,50);
    ui->textBrowser->move(7,127);
    connect(ui->graphicsView,SIGNAL(doubleClickedImage()),this,SLOT(doubleClickCover()));
    src = new WebSearch();
    crpl = new AddNewPlaylist();
    settingsForm = new SimplySettings();

    LoadSkin();

    QRect scr = QApplication::desktop()->screenGeometry();
    crpl->move(scr.center() - rect().center());
    crpl->close();

  //  src->move(scr.center() - rect().center());
  //  src->close();

   // settingsForm->move(scr.center() - rect().center());
   // settingsForm->close();



    LoadPlayerSettings();
    PlaylistTimer = new QTimer(this);
    LabelTimer = new QTimer(this);
    ResizeTimer = new QTimer(this);
    SignalsTimer = new QTimer(this);
    lower = new QTimer(this);
    SpectrumTimer = new QTimer(this);
    DownloadTimer = new QTimer(this);
    //newthread = new QTimerThread;
    //newthread->start();

    connect(PlaylistTimer, SIGNAL(timeout()), this, SLOT(PlaylistTimerEvent()));
    //connect(newthread->timer, SIGNAL(timeout()), this, SLOT(LabelTimerEvent()));
    connect(LabelTimer, SIGNAL(timeout()) , SLOT(LabelTimerEvent()));
    connect(ResizeTimer, SIGNAL(timeout()), this, SLOT(ResizeTimerEvent()));
    connect(SignalsTimer, SIGNAL(timeout()), this, SLOT(SignalsTimerEvent()));
    connect(lower, SIGNAL(timeout()), this, SLOT(LowerTimerEvent()));
    connect(SpectrumTimer, SIGNAL(timeout()), this, SLOT(SpectrumTimerEvent()));
    connect(DownloadTimer, SIGNAL(timeout()), this, SLOT(downloadTimerEvent()));
   // connect(ui->trackbar,SIGNAL(clicked()),this,SLOT(onClickTrackBar));


    ResizeTimer->start(RTTIME);

    state = STATE_NAME_1;
    Counter = 0;
    percent = 100;

    ui->tabWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QFontDatabase::addApplicationFont( "data\\marcopolo_13x9.ttf");
    QFontDatabase::addApplicationFont( "data\\LCD.ttf");

    Simply_Hint = new SimplyHint();
    Simply_Hint->setWindowFlags(playlistDock->windowFlags() | Qt::SubWindow);
    CreateMirrorClock();
    SignalsTimer->start(100);

     newTab = new QTab(ui->tabWidget);
     ui->tabWidget->addTab(newTab, "temp");
     QRect size = QApplication::desktop()->rect();
     LoadingScreen *screen = new LoadingScreen(0,"Loading playlist");
     screen->show();
     ParsePlaylists();
     delete screen;


     if(this->pos().x() < 0 || this->pos().x() < 0)
         this->move(0,0);

     ui->centralWidget->setMouseTracking(true);

     QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();

      for(int i = 0; i < allTabs.count();i++)
      {
          allTabs[i]->setContextMenuPolicy(Qt::CustomContextMenu);
          connect(allTabs[i], SIGNAL(customContextMenuRequested(const QPoint&)),
              this, SLOT(showPlaylistContextMenu(const QPoint&)));
          allTabs[i]->setFixedHeight(ui->centralWidget->height()-220);
      }

      playlistDock->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(playlistDock, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(playlistDockContextMenu(const QPoint&)));

      albumInfo->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(albumInfo, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(albumInfoContextMenu(const QPoint&)));


      SimplyHotkeyManager *hotkey = new SimplyHotkeyManager();
      ui->statusBar->addWidget(ui->findEdit,0);

      connect(ui->widget_3,SIGNAL(expandClicked()),this,SLOT(expandPlaylist()));
      connect(ui->widget_3,SIGNAL(collapseClicked()),this,SLOT(expandPlaylist()));

      //ui->graphicsView->resize(100,100);
      //ui->textBrowser->resize(100,100);
      //ui->textBrowser->move(5,101);
     // ui->widget_3->setLabelText("Album info");
      afterInit = true;
      LoadPlayerSettings();

      ui->widget_3->setHidden(true);
      ui->Balance->setHidden(true);
      ui->Ballabel->setHidden(true);



      QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);

      settings.beginGroup("Playlists");
      int count = settings.value("Number", 1).toInt();

      allTabs = playlistDock->findChildren<QPlaylist *>();

      for(int i = 0; i < allTabs.count(); i++)
      {
          if(settings.value("LastPlaylist", 1).toString() == allTabs[i]->GetPlaylistName())
          {
               //QMessageBox::about(0,QString("543"),tr("k"));
               currentPlaylist = allTabs[i];
               break;
          }

      }

      initMenus();




      this->show();
      this->setAcceptDrops(true);

      playlistDock->setWindowFlags(playlistDock->windowFlags() | Qt::SubWindow);
      albumInfo->setWindowFlags(albumInfo->windowFlags() | Qt::SubWindow);
     // playlistDock->setAttribute(Qt::WA_OpaquePaintEvent,true);
     // playlistDock->setAutoFillBackground(false);

      movePlaylistWindow();
      moveAlbumInfoWindow();
     // playlistDock->setAcceptDrops(true);
     // playlistDock->resize(playlistDock->geometry().size());
      //playlistDock->repaint();
      albumInfo->show();
      playlistDock->show();
      playlistSkin->updateStyle();
      playlistSkin->repaint();
   //   qDebug() << currentPlaylist->GetPlaylistName();

      QSettings sett(CONFIG_PATH, QSettings::IniFormat);//("RavanBladeSoftware", "SimplyPlayer");
      sett.beginGroup("MainWindow");
      playlistDock->setHidden(sett.value("playlist_hidden", false).toBool());
      albumInfo->setHidden(sett.value("albuminfo_hidden", false).toBool());
      sett.endGroup();

      loadPlugins();

     // LoadPlayerSettings();





    // playlistDock->setWindowFlags(playlistDock->windowFlags() | Qt::Dialog);
    // albumInfo->setWindowFlags(albumInfo->windowFlags() | Qt::Dialog);


     // QYouTubeParser *par = new QYouTubeParser("http://www.youtube.com/watch?v=RCqEkWY8ekQ");
    //  FileDownloader *yout = new FileDownloader(QUrl("http://www.youtube.com/watch?v=RCqEkWY8ekQ"));

      //par.

    setupThumbButtons();

    bufferEnabled = true;

 }

SimplyWindow::~SimplyWindow()
{
    delete ui;
}


void SimplyWindow::loadPlugins()
{
    QStringList nameFilter("*.dll");
    QDir directory(QDir::fromNativeSeparators(APPLICATION_PATH + "data//plugins//"));
    QStringList files = directory.entryList(nameFilter);

    for(int i = 0; i < files.count(); i++)
        Player->LoadPlugin(files[i]);


}


void SimplyWindow::setupThumbButtons()
{


    QIcon playBtt = QIcon(APPLICATION_PATH +"data/thumbbuttons/Play.png");
    QIcon stopBtt = QIcon(APPLICATION_PATH + "data/thumbbuttons/Stop.png");
    QIcon pauseBtt = QIcon(APPLICATION_PATH + "data/thumbbuttons/Pause.png");


    thumbbar = new QWinThumbnailToolBar(this);
    thumbbar->setWindow(this->windowHandle());

    playthumb = new QWinThumbnailToolButton(thumbbar);
    playthumb->setToolTip("Play");

    playthumb->setIcon(QIcon(QDir::fromNativeSeparators(APPLICATION_PATH +"data/temp/thumbbuttons/Play.png")));
    playthumb->setDismissOnClick(true);
    connect(playthumb, SIGNAL(clicked()), this, SLOT(on_Play_clicked()));

    QWinThumbnailToolButton *pausethumb = new QWinThumbnailToolButton(thumbbar);
    pausethumb->setToolTip("Pause");
    pausethumb->setIcon(QIcon(QDir::fromNativeSeparators(APPLICATION_PATH + "data/temp/thumbbuttons/Pause.png")));
    pausethumb->setDismissOnClick(true);
    connect(pausethumb, SIGNAL(clicked()), this, SLOT(on_Play_clicked()));


    stopthumb = new QWinThumbnailToolButton(thumbbar);
    stopthumb->setToolTip("Stop");
    stopthumb->setIcon(QIcon(QDir::fromNativeSeparators(APPLICATION_PATH + "data/temp/thumbbuttons/Stop.png")));
    stopthumb->setDismissOnClick(true);
    connect(pausethumb, SIGNAL(clicked()), this, SLOT(on_Stop_clicked()));

    thumbbar->addButton(playthumb);
    //thumbbar->addButton(pausethumb);
    thumbbar->addButton(stopthumb);

}

void SimplyWindow::initMenus()
{
    //Pierwszy poziom menu
    QAction *musicdown = new QAction(QString("Search music"),&mainMenu);
    QAction *settings = new QAction(QString("Settings"),&mainMenu);
    QAction *about = new QAction(QString("About"),&mainMenu);
    QAction *exit = new QAction(QString("Exit"),&mainMenu);


    //Submenu widgety
    QMenu *widgets = new QMenu(QString("Widgets"),&mainMenu);

    //Okna
    playlista = new QAction(QString("Playlist"),widgets);
    albuminf = new QAction(QString("Album Info"),widgets);

    playlista->setCheckable(true);
    albuminf->setCheckable(true);

    playlista->setChecked(!playlistDock->isHidden());
    albuminf->setChecked(!albumInfo->isHidden());

    connect(playlista, SIGNAL(triggered()), this, SLOT(checkPlaylistWidget()));
    connect(albuminf, SIGNAL(triggered()), this, SLOT(checkAlbumInfoWidget()));

    widgets->addAction(playlista);
    widgets->addAction(albuminf);

    musicdown->setIconVisibleInMenu(false);
    connect(musicdown, SIGNAL(triggered()), this, SLOT(searchMusic()));
    connect(settings, SIGNAL(triggered()), this, SLOT(openSettings()));
    mainMenu.addAction(musicdown);
    mainMenu.addAction(settings);
    mainMenu.addMenu(widgets);
    mainMenu.addSeparator();
    mainMenu.addAction(about);
    mainMenu.addAction(exit);

}

void SimplyWindow::LoadSkin()
{

    QSettings settings(CONFIG_PATH, QSettings::IniFormat);//("RavanBladeSoftware", "SimplyPlayer");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    playlistDock->resize(settings.value("playlist_size", QSize(800, 800)).toSize());
    playlistDock->move(settings.value("playlist_pos", QPoint(200, 200)).toPoint());
    albumInfo->resize(settings.value("albuminfo_size", QSize(800, 800)).toSize());
    albumInfo->move(settings.value("albuminfo_pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
    Player_Skin = new QSkinObject(this,loadedGraphic,false);
    Player_Skin->startSkinning();
    connect(Player_Skin,SIGNAL(barButtonClicked()),this,SLOT(barButtonClick()));
    connect(Player_Skin,SIGNAL(closeClicked()),this,SLOT(closeMainWindow()));
    connect(Player_Skin,SIGNAL(minClicked()),this,SLOT(minMainWindow()));
    connect(Player_Skin,SIGNAL(maxClicked()),this,SLOT(maxMainWindow()));

    playlistSkin = new QSkinObject(playlistDock,loadedGraphic,false);
    playlistSkin->startSkinning();

    connect(playlistSkin,SIGNAL(closeClicked()),this,SLOT(closePlaylistWindow()));
    connect(playlistSkin,SIGNAL(minClicked()),this,SLOT(minPlaylistWindow()));
    connect(playlistSkin,SIGNAL(maxClicked()),this,SLOT(maxPlaylistWindow()));

    playlistSkin->setLabelName("Playlists");
    playlistSkin->hideButton(MENU_BUTTON,true);
    playlistSkin->hideButton(MINIMIZE_BUTTON,true);
    playlistSkin->hideButton(MAXIMIZE_BUTTON,true);

    albuminfoSkin = new QSkinObject(albumInfo,loadedGraphic,false);
    albuminfoSkin->startSkinning();
    albuminfoSkin->hideButton(MENU_BUTTON,true);
    albuminfoSkin->hideButton(MINIMIZE_BUTTON,true);
    albuminfoSkin->hideButton(MAXIMIZE_BUTTON,true);
    albuminfoSkin->setLabelName("Album info");
    connect(albuminfoSkin,SIGNAL(closeClicked()),this,SLOT(closeAlbumInfoWindow()));
   // albuminfoSkin->
    albumInfo->setAttribute(Qt::WA_TranslucentBackground,true);
    playlistDock->setAttribute(Qt::WA_TranslucentBackground,true);

    //albumInfo->setWindowFlags(albumInfo->windowFlags() | Qt::Tool);
   // playlistDock->setWindowFlags(playlistDock->windowFlags() | Qt::Tool);
    albumInfo->show();


    playlistDock->show();

    playlistDock->setMinimumSize(150,150);
    albumInfo->setMinimumSize(150,130);

    alphaPL = playlistSkin->getFrameAlpha();
    alphaAI = albuminfoSkin->getFrameAlpha();
    playlistDock->setHidden(settings.value("playlist_hidden", false).toBool());
    albumInfo->setHidden(settings.value("albuminfo_hidden", false).toBool());

    newplaylistSkin = new QSkinObject(crpl,loadedGraphic,false);
    newplaylistSkin->startSkinning();
    newplaylistSkin->hideButton(MENU_BUTTON,true);
    newplaylistSkin->hideButton(MAXIMIZE_BUTTON,true);
    newplaylistSkin->hideButton(MINIMIZE_BUTTON,true);
    newplaylistSkin->setLabelName("Create New Playlist");

    crpl->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("All.qss")));
    connect(crpl,SIGNAL(addPlaylistSignal(QString,QString,QString)),this,SLOT(createNewPlaylist(QString, QString, QString)));
    connect(crpl,SIGNAL(reloadPlaylist(QString,QString,QPlaylist*,bool)),this,SLOT(reloadPlaylist(QString, QString, QPlaylist*,bool)));

    settingsSkin = new QSkinObject(settingsForm,loadedGraphic,false);
    settingsSkin->startSkinning();
    settingsSkin->hideButton(MENU_BUTTON,true);
    settingsSkin->hideButton(MAXIMIZE_BUTTON,true);
    settingsSkin->hideButton(MINIMIZE_BUTTON,true);
    settingsSkin->setLabelName("SimplyPlayer Settings");
    settingsForm->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("All.qss")));
    settingsForm->setTopBarHeight(loadedGraphic->getContainer()->at(0).height());
    settingsForm->close();
   // settingsForm->
    searchSkin = new QSkinObject(src,loadedGraphic,false);
    searchSkin->startSkinning();
    searchSkin->hideButton(MENU_BUTTON,true);
    searchSkin->hideButton(MAXIMIZE_BUTTON,true);
    searchSkin->hideButton(MINIMIZE_BUTTON,true);
    searchSkin->setLabelName("Search Music");
    src->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("All.qss")));
    //src->setTopBarHeight(loadedGraphic->getContainer()->at(0).height());
    src->close();



  //  albumInfo->setWindowOpacity(0.5);
   // playlistDock->setWindowOpacity(1);
  //  albuminfoSkin->setFrameAlpha(0.5);


}


void SimplyWindow::expandPlaylist()
{
    if(ui->widget_3->isExpanded())
    {
       // ui->tabWidget->currentWidget()->resize(ui->tabWidget->width(),ui->tabWidget->height()+50);
      //  ui->tabWidget->resize(ui->tabWidget->width(),ui->tabWidget->height()+50);
        this->resize(this->width(),this->height()+120);
    }
    else
    {
      //  ui->tabWidget->resize(ui->tabWidget->width(),ui->tabWidget->height()+50);
            this->resize(this->width(),this->height()-120);
    }
       // Player_Skin->updateStyle();
       // Player_Skin->repaint();
}

void SimplyWindow::ParsePlaylists()
{
    QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);
    int a = 0;
    settings.beginGroup("Playlists");
    int count = settings.value("Number", 1).toInt();
    qDebug() << "Playlist parser: " << a++;
    for(int i = 0; i < count; i++)
    {
        if(settings.value("File"+QString(i), "playlist.pls").toString().capacity() > 0)            
        {
            addNewPlaylist(settings.value("Playlist"+QString::number(i), "playlist").toString(),settings.value("File"+QString::number(i), "playlist.pls").toString());
        }
    }
qDebug() << "Playlist parser: " << a++;

    QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();

    for(int i = 0; i < allTabs.count(); i++)
    {
        if(settings.value("LastPlaylist", 1).toString() == allTabs[i]->GetPlaylistName())
        {
             //QMessageBox::about(0,QString("543"),tr("k"));
             currentPlaylist = allTabs[i];
             break;
        }

    }

qDebug() << "Playlist parser: " << a++;
    delete newTab;
qDebug() << "Playlist parser: " << a++;
    QList<QTab *> allTabs2 = playlistDock->findChildren<QTab *>();
    bool found = false;
qDebug() << "Playlist parser: " << a++;

    for(int j = 0; j < allTabs2.count(); j++)
    {
        if(settings.value("LastPlaylist", 1).toString() == allTabs2[j]->GetTabName())
        {
              int index = ui->tabWidget->indexOf(allTabs2[j]);

              QLabel *button1 = new QLabel();
              button1->setText(tr("    ") + ui->tabWidget->tabBar()->tabText(index));
              button1->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("ActiveTab.qss")));
              ui->tabWidget->tabBar()->setTabButton(index, QTabBar::LeftSide,((QWidget*)(button1)));
              ui->tabWidget->tabBar()->setTabText(index,"");
              ui->tabWidget->tabBar()->tabButton(index,QTabBar::LeftSide)->move(ui->tabWidget->tabBar()->tabButton(index,QTabBar::LeftSide)->geometry().x(),ui->tabWidget->tabBar()->tabButton(index,QTabBar::LeftSide)->geometry().y()-2);
              found = true;
              break;
        }
    }
qDebug() << "Playlist parser: " << a++;
    if(found == false)
    {
        for(int a = 0; a < allTabs2.count(); a++)
        {

                  int index = ui->tabWidget->indexOf(allTabs2[a]);
                  settings.setValue("LastPlaylist",allTabs2[a]->GetTabName());

                  QLabel *button1 = new QLabel();
                  button1->setText(tr("    ") + ui->tabWidget->tabBar()->tabText(index));
                  button1->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("ActiveTab.qss")));
                  ui->tabWidget->tabBar()->setTabButton(index, QTabBar::LeftSide,((QWidget*)(button1)));
                  ui->tabWidget->tabBar()->setTabText(index,"");
                  ui->tabWidget->tabBar()->tabButton(index,QTabBar::LeftSide)->move(ui->tabWidget->tabBar()->tabButton(index,QTabBar::LeftSide)->geometry().x(),ui->tabWidget->tabBar()->tabButton(index,QTabBar::LeftSide)->geometry().y()-2);
                  break;
        }
    }
qDebug() << "Playlist parser: " << a++;

    settings.endGroup();

}

void SimplyWindow::SetupStyleSheets()
{

   this->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("All.qss")));
   ui->Backward->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Backward.qss")));
   ui->Balance->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Balance.qss")));
   ui->Ballabel->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Balance_label.qss")));
   ui->centralWidget->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Central_Widget.qss")));
   ui->Forward->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Forward.qss")));
   ui->frame->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Frame.qss")));
   //ui->Open->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Open.qss")));
   ui->Border->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Border.qss")));
   ui->Panel->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Panel.qss")));
   //ui->Panel2->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("SpectrumPanel.qss")));
   //ui->Pause->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Pause.qss")));
   ui->Play->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Play.qss")));
   ui->SongName->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Songname_label.qss")));
   ui->SpectrumPanel->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Spectrum_panel.qss")));
   ui->Stop->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Stop.qss")));
   ui->tabWidget->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Tab_widget.qss")));
   ui->trackbar->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Trackbar.qss")));
   ui->Vollabel->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Volume_label.qss")));
   ui->Volume->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Volume.qss")));
   ui->textBrowser->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("AlbumDescription.qss")));



}

void SimplyWindow::SavePlayerSettings()
{
    QSettings settings(CONFIG_PATH, QSettings::IniFormat);//("RavanBladeSoftware", "SimplyPlayer");

    settings.beginGroup("MainWindow");

  //  short int mod = ui->widget_3->isExpanded() ? 120 : 0;
    QSize siz = QSize(this->width(),this->height());
    settings.setValue("size", siz);
    settings.setValue("pos", pos());
    settings.setValue("volume", ui->Volume->value());

    int ind = 0;

    for(int i = 0; i < currentPlaylist->topLevelItemCount(); i++)
        if(currentPlaylist->topLevelItem(i)->data(2,0) == Player->GetLastFileName())
            ind = i;


    settings.setValue("last_index", ind < 0 ? -1 : ind);
    settings.setValue("last_range_min", ui->trackbar->minimum());
    settings.setValue("last_range_max", ui->trackbar->maximum());
    settings.setValue("last_trackbar_value", ui->trackbar->value());

    QSize size = QSize(playlistDock->width(),playlistDock->height());
    settings.setValue("playlist_size", size);
    settings.setValue("playlist_pos", playlistDock->pos());
    settings.setValue("playlist_dockanchor",!playlistSkin->getResizeHandler()->isMovingEnabled()); // Negacja bo funkcja zwarca czy mozna ruszac

    size = QSize(albumInfo->width(),albumInfo->height());
    settings.setValue("albuminfo_size", size);
    settings.setValue("albuminfo_pos", albumInfo->pos());

    settings.setValue("albuminfo_dockanchor",!albuminfoSkin->getResizeHandler()->isMovingEnabled());

   // settings.setValue("playlist_hidden", !playlistDock->isHidden());
   // settings.setValue("albuminfo_hidden", !albumInfo->isHidden());

    settings.endGroup();
}

void SimplyWindow::LoadPlayerSettings()
{
    QSettings settings(CONFIG_PATH, QSettings::IniFormat);//("RavanBladeSoftware", "SimplyPlayer");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
   // youtubedl_path = settings.value("youtubedl_path", "").toString();
    youtubedl_path = QDir::fromNativeSeparators(APPLICATION_PATH + "data\\modules\\youtube-dl.exe");
    int settingsVolume = settings.value("volume", 100).toInt();
    ui->Volume->setValue(settingsVolume);
    ui->Vollabel->setText("Volume: " + QString::number(settingsVolume) + "%");
    Player->SetVolume(settingsVolume);

    if(afterInit)
    {
        playlistDock->resize(settings.value("playlist_size", QSize(800, 800)).toSize());
        playlistDock->move(settings.value("playlist_pos", QPoint(200, 200)).toPoint());
        playlistSkin->getResizeHandler()->setActive(Move,!(settings.value("playlist_dockanchor", false).toBool()));
        albumInfo->resize(settings.value("albuminfo_size", QSize(800, 800)).toSize());
        albumInfo->move(settings.value("albuminfo_pos", QPoint(200, 200)).toPoint());
        albuminfoSkin->getResizeHandler()->setActive(Move,!(settings.value("albuminfo_dockanchor", false).toBool()));
        albuminfoSkin->getResizeHandler()->setMovingEnabled(!(settings.value("albuminfo_dockanchor", false).toBool()));
        playlistSkin->getResizeHandler()->setMovingEnabled(!(settings.value("playlist_dockanchor", false).toBool()));
        playlistDock->setHidden(settings.value("playlist_hidden", false).toBool());
        albumInfo->setHidden(settings.value("albuminfo_hidden", false).toBool());

        last_index = settings.value("last_index", 0).toInt();
        if(last_index != -1 && currentPlaylist->topLevelItemCount() != 0 && !SimplyFunctions::isHTTP(currentPlaylist->topLevelItem(last_index)->data(3,0).toString()))
        {

        ui->trackbar->setRange(settings.value("last_range_min", 0).toInt(),settings.value("last_range_max", 0).toInt());

        doubleClicked(currentPlaylist->topLevelItem(last_index), 3);

        Player->ChangePosition(settings.value("last_trackbar_value", 0).toInt());
        ui->trackbar->setValue(settings.value("last_trackbar_value", 0).toInt());

        ui->SongName->move(290,ui->SongName->y());

        Player->Pause();
        }

    }

    Player->SetVolume(settings.value("volume", 100).toInt());
    //repeat_index = settings.value("repeatindex", -1).toInt();

    //if(useSkin)
       // this->setWindowOpacity(Player_Skin->alpha);
    //else
        this->setWindowOpacity( settings.value("AlphaBlending", 1).toDouble());


    settings.endGroup();

    QSettings settings2(SKINCONFIG_PATH, QSettings::IniFormat);
    settings2.beginGroup("SkinFile");
    useSkin = settings2.value("use_Skin", true).toBool();
    useGlassEffect = settings2.value("use_GlassEffect", false).toBool();
    playlist_font.fromString(settings.value("Playlist_font", QFont("Tahoma")).toString());
    playlist_font.setPixelSize(settings.value("Playlist_font_size", 10).toInt());
    playlist_font.setStyleStrategy(QFont::PreferAntialias);
    highlightColor = QColor(settings2.value("HighlightColor_R", 255).toInt(),settings2.value("HighlightColor_G", 255).toInt(),settings2.value("HighlightColor_B", 255).toInt(),settings2.value("HighlightColor_Alpha", 255).toInt());
    styleHighlightColor = QColor(settings2.value("PlaylistFontColor_R", 255).toInt(),settings2.value("PlaylistFontColor_G", 255).toInt(),settings2.value("PlaylistFontColor_B", 255).toInt(),settings2.value("PlaylistFontColor_Alpha", 255).toInt());
    QFont font;
    font.fromString(settings2.value("Songname_font", QFont("Tahoma")).toString());
    font.setPixelSize(10);
    font.setStyleStrategy(QFont::PreferAntialias);
    ui->SongName->setFont(font);

    if(afterInit)
    {
        Player_Skin->setFrameAlpha(settings2.value("MainWindowFrameAlpha", 0.8).toDouble());
        playlistSkin->setFrameAlpha(settings2.value("PlaylistFrameAlpha", 0.8).toDouble());
        albuminfoSkin->setFrameAlpha(settings2.value("AlbumInfoFrameAlpha", 0.8).toDouble());
    }

    settings2.endGroup();



    SetupStyleSheets();
}

void SimplyWindow::playlistDockContextMenu(const QPoint& pos)
{
    QPoint globalPos = playlistDock->mapToGlobal(pos);
    QMenu myMenu;
    QAction *act = new QAction("Dock",&myMenu);
    act->setCheckable(true);
    act->setChecked(!playlistSkin->getResizeHandler()->isMovingEnabled());
    myMenu.addAction(act);

    if(!playlistDock->isDocked())
        act->setEnabled(false);

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if(selectedItem->text() == "Dock")
        {
            playlistSkin->getResizeHandler()->setActive(Move,!playlistSkin->getResizeHandler()->isActive());
            playlistSkin->getResizeHandler()->setMovingEnabled(!playlistSkin->getResizeHandler()->isMovingEnabled());

        }

    }
}

void SimplyWindow::albumInfoContextMenu(const QPoint& pos)
{
    QPoint globalPos = albumInfo->mapToGlobal(pos);
    QMenu myMenu;
    QAction *act = new QAction("Dock",&myMenu);
    act->setCheckable(true);
    act->setChecked(!albuminfoSkin->getResizeHandler()->isMovingEnabled());

    if(!albumInfo->isDocked())
        act->setEnabled(false);

    myMenu.addAction(act);
    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if(selectedItem->text() == "Dock")
        {
            albuminfoSkin->getResizeHandler()->setActive(Move,!albuminfoSkin->getResizeHandler()->isActive());
            albuminfoSkin->getResizeHandler()->setMovingEnabled(!albuminfoSkin->getResizeHandler()->isMovingEnabled());

        }

    }
}

void SimplyWindow::showPlaylistContextMenu(const QPoint& pos)
{
    QTab *widget = (QTab *)(ui->tabWidget->currentWidget());
    QList<QPlaylist *> playlist = widget->findChildren<QPlaylist *>();

    QPoint globalPos = playlist[0]->mapToGlobal(pos);
    // for QAbstractScrollArea and derived classes you would use:
    // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

    QMenu myMenu;
    myMenu.addAction("Play");
    myMenu.addAction("Delete");
    myMenu.addSeparator();
    myMenu.addAction("Locate");
    myMenu.addSeparator();
    myMenu.addAction("Add file");
    myMenu.addSeparator();
    myMenu.addAction("Download");

   // myMenu.addAction("Add link");

    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if(selectedItem->text() == "Play")
        {
            doubleClicked(playlist[0]->currentItem(), 0);
        }
        else if(selectedItem->text() == "Delete")
        {
            onDeleteItem();
        }
        else if(selectedItem->text() == "Locate")
        {
            if(!SimplyFunctions::isHTTP(playlist[0]->currentItem()->text(3)))
            {
                QString path = QDir::toNativeSeparators(playlist[0]->currentItem()->text(3));
                QFileInfo inf = QFileInfo(path);
                QDesktopServices::openUrl(QUrl("file:///" + inf.absolutePath()));
            }
            else
            {
                QDesktopServices::openUrl(QUrl(playlist[0]->currentItem()->text(3)));
            }

        }
        else if(selectedItem->text() == "Add file")
        {
            this->on_Open_clicked();
        }
        else if(selectedItem->text() == "Add link")
        {
            addlink *link = new addlink(this);
            link->show();
        }
        else if(selectedItem->text() == "Download")
        {
          //  if(SimplyFunctions::isHTTP(playlist[0]->currentItem()->text(3)))
         //   {
                QString path = playlist[0]->currentItem()->text(3);
                youtube_dl = new QProcess();
                youtube_dl->start( youtubedl_path, QStringList() << path << "-o" << playlist[0]->currentItem()->text(2) + ".mp4" << "--max-quality=18" << "-f" << "mp4" );
               // QString a;
               // a = path + " -o" + youtubedl_path +"/"+ playlist[0]->currentItem()->text(2) + ".mp4" + "--max-quality=18" + "-f" + "mp4";
              //  qDebug() << a;
              //  connect(youtube_dl, SIGNAL(finished(int)), this, SLOT(youtubedlProcessFinished(int)));

               QColor rowColor = Qt::blue;
                //playlist[0]->currentItem()->setData(atoi(playlist[0]->currentItem()->data(1,0).toString().toLatin1().data())-1,Qt::BackgroundRole, rowColor);
                playlist[0]->currentItem()->setBackgroundColor(1,Qt::green);
                playlist[0]->currentItem()->setBackgroundColor(2,Qt::green);
                playlist[0]->currentItem()->setBackgroundColor(4,Qt::green);
                playlist[0]->currentItem()->setBackgroundColor(5,Qt::green);
                playlist[0]->currentItem()->setBackgroundColor(6,Qt::green);
                playlist[0]->currentItem()->setBackgroundColor(7,Qt::green);
                QSignalMapper mapper;
                connect(youtube_dl, SIGNAL(finished(int)), &mapper, SLOT(map()));
                mapper.setMapping(youtube_dl, atoi(playlist[0]->currentItem()->data(1,0).toString().toLatin1().data()-1));
                connect(&mapper, SIGNAL(mapped(int)), this, SLOT(youtubedlDownloadFinished(int)));




        }

    }
    else
    {

    }

}
QString SimplyWindow::ConvertWrzutaAddress(QString url)
{

    QStringList split = url.split("/");

    QStringList split2 = split[2].split("\\.");

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int randomValue = qrand() % ((999999 + 1) - 0) + 0;

    QString url2 = "http://" + split2[0] + "/xml/plik/" + split[4] + "/wrzuta.pl/pl/" + QString::number(randomValue);

    qDebug() << url2;

    return url2;


}


SIMPLY_PLAYLIST SimplyWindow::AddFileToPlaylist(QString Path, QPlaylist *playlist, QString time, QString name)
{

    QFileInfo pathInfo(Path);

    QUrl url(Path);




  //  if(!pathInfo.exists())
  //      return INVALID_PATH;

  //  if(!IsSupportedFormat(pathInfo.completeSuffix()))
  //      return FORMAT_NOT_SUPPORTED;

    int count = playlist->topLevelItemCount() + 1;



    QTreeWidgetItem *Item = new QTreeWidgetItem;
    FileDownloader *wrzuta;

        Item->setText(0,"");
        Item->setText(1,QString::number(count));

        if(name.length() < 1)
        {
            if(SimplyFunctions::isHTTP(Path))
            {
                if(SimplyFunctions::isWrzutaAd(Path))
                {
                  /*QString a = ConvertWrzutaAddress(Path);
                    wrzuta = new FileDownloader(QUrl(a));

                    QEventLoop loop;
                    connect(wrzuta, SIGNAL(downloaded()), &loop, SLOT(quit()));
                    loop.exec();

                    Item->setText(2,WrzutaD->getFName(wrzuta->downloadedData()));*/
                }
                else if(SimplyFunctions::isYoutubeAd(Path))
                {


                }
            }
            else
            {
                Item->setText(2,pathInfo.fileName());
            }
        }
        else
        {
            Item->setText(2,name);
        }

        Item->setText(3,Path);

       // qDebug() << time;
        if((time.length() < 1) || (time == "0:0-1"))
        {
            PTime Time;
            bool setTime = true;

            if(SimplyFunctions::isHTTP(Path))
            {
                if(SimplyFunctions::isWrzutaAd(Path))
                {
                /*    QString a = ConvertWrzutaAddress(Path);
                    wrzuta = new FileDownloader(QUrl(a));

                    QEventLoop loop;
                    connect(wrzuta, SIGNAL(downloaded()), &loop, SLOT(quit()));
                    loop.exec();

                    if(wrzuta->downloadedData().length() > 20)
                        Time = Player->GetPTime(WrzutaD->getFName(wrzuta->downloadedData()));
                    else
                        setTime = false;*/
                }
                else if(SimplyFunctions::isYoutubeAd(Path))
                {

                    time="0:0-1";

                    setTime = false;
                }
            }
            else
                Time = Player->GetPTime(Path);

            if(setTime)
                Item->setText(4,QString::number((int)Time.minutes) + ":" + OneToTwo(Time.seconds));
            else
                Item->setText(4,time);
        }
        else
        {
            Item->setText(4,time);
        }
        Item->setText(5,"");

        playlist->insertTopLevelItem(playlist->topLevelItemCount(),Item);
   // }

    QPushButton *delegateDelete = new QPushButton();
    QPushButton *delegateRepeat = new QPushButton();
    delegateDelete->setMaximumSize(20,15);
    delegateRepeat->setMaximumSize(20,15);
    delegateDelete->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("DeleteDelegateItem.qss")));
    delegateRepeat->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItem.qss")));

    connect(delegateDelete, SIGNAL(clicked()), this, SLOT(onDeleteItem()));
    connect(delegateRepeat, SIGNAL(clicked()), this, SLOT(setRepeat()));

    playlist->setItemWidget(Item,5,delegateDelete);
    playlist->setItemWidget(Item,6,delegateRepeat);
    playlist->RefreshPlaylist();

}



void SimplyWindow::addNewPlaylist(QString name,QString file_path)
{
    QTab *newTab = new QTab(ui->tabWidget);
    ui->tabWidget->addTab(newTab, name);
    QPlaylist *newpl = new QPlaylist(newTab);
    QTreeWidgetItem *itemo = new QTreeWidgetItem;
    itemo->setText(0,tr("1"));
    itemo->setText(1,tr("2"));
    itemo->setText(3,tr("3"));
    itemo->setText(4,tr("4"));
    itemo->setText(5,tr("5"));
    itemo->setText(6,tr("6"));
    newpl->setHeaderItem(itemo);
    newpl->setColumnWidth(0,0);
    newpl->setColumnWidth(2,180); // 200
    newpl->setColumnWidth(3,0);
    newpl->setColumnWidth(4,20);
    newpl->setColumnWidth(1,15);
    newpl->setColumnWidth(5,10);
    newpl->setColumnWidth(6,10);

    newpl->setMouseTracking(true);
    newpl->SetPlaylistName(name);
    newpl->SetPlaylistPath(file_path);
    newTab->SetTabName(name);
    newpl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    newpl->setFont(playlist_font);
    newpl->setAcceptDrops(true);
    newpl->setDragEnabled(true);
    newpl->setGeometry(9,9,ui->tabWidget->width()-20,ui->tabWidget->height()-60);
    newpl->setHeaderHidden(true);
    newpl->setAttribute(Qt::WA_Hover);
    newpl->setFrameShape(QFrame::NoFrame);
    newpl->setFrameShadow(QFrame::Plain);
    connect(newpl, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleClicked(QTreeWidgetItem*, int)));
    newpl->setUniformRowHeights(true);
    newpl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    newpl->setFocusPolicy(Qt::NoFocus);
    newpl->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Playlist.qss")));

    LoadPLS(file_path, newpl);

}

SIMPLY_PLS SimplyWindow::LoadPLS(QString path, QPlaylist *playlist)
{
    QSettings settings(QDir::fromNativeSeparators(QApplication::applicationDirPath() + "/" + path), QSettings::IniFormat);
    //qDebug() << QApplication::applicationDirPath() + "/" + path;
    QFileInfo pathInfo(path);
  //  if(!pathInfo.exists())
  //      return INVALID_DIR;

    settings.beginGroup("playlist");

    //settings.value("NumberOfEntries",0).toInt();//


      //  return NO_ENTRIES;

    for(int i = 0; i < settings.value("NumberOfEntries",0).toInt(); i++)
    {
      //  qDebug() << settings.value("File"+QString::number(i), "knockknock").toString();
        AddFileToPlaylist(settings.value("File"+QString::number(i), "knockknock").toString(),playlist, settings.value("Time"+QString::number(i), "").toString(), settings.value("Title"+QString::number(i), "").toString());
    }


    if(settings.contains("RepeatIndex"))
    {
        playlist->SetPlaylistRepeatIndex(settings.value("RepeatIndex",-1).toInt());
        if(settings.value("RepeatIndex",-1).toInt() > -1)
            playlist->itemWidget(playlist->topLevelItem(playlist->GetPlaylistRepeatIndex()),6)->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItemSet.qss")));
    }

    settings.endGroup();

}

SIMPLY_PLS SimplyWindow::SavePLS(QString path, QPlaylist *playlist)
{
    QSettings settings(path, QSettings::IniFormat);

    QFileInfo pathInfo(path);
    if(!pathInfo.exists())
       return INVALID_DIR;

    settings.beginGroup("playlist");
    settings.setValue("NumberOfEntries", playlist->topLevelItemCount());
    settings.setValue("RepeatIndex", playlist->GetPlaylistRepeatIndex());
   // settings.setValue("Description", ui->textBrowser->toPlainText());
   // ui->graphicsView->

    for(int i = 0; i < settings.value("NumberOfEntries",0).toInt(); i++)
    {
        settings.setValue("file"+QString::number(i), playlist->topLevelItem(i)->data(3,0));
        settings.setValue("title"+QString::number(i), playlist->topLevelItem(i)->data(2,0));
        settings.setValue("time"+QString::number(i), playlist->topLevelItem(i)->data(4,0));
    }


    settings.endGroup();
}

void SimplyWindow::SavePlaylist(QString playlist_name, QString playlist_file, int number)
{

    QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);

    settings.beginGroup("Playlists");

    QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();

    settings.setValue("Number", allTabs.count());
    //settings.setValue("LastPlaylist", tr(currentPlaylist->metaObject()->className()));

   //  for(int i = 0; i < allTabs.count();i++)
  //   {
         settings.setValue("Playlist"+QString::number(number), playlist_name);
         settings.setValue("File"+QString::number(number), QDir::fromNativeSeparators(QApplication::applicationDirPath() + "/" + playlist_file));
        // settings.setValue("Description", ui->textBrowser->toPlainText());
   //  }

    settings.endGroup();
}

void SimplyWindow::AddEntryToPlaylistFile(QString playlist_name, QString playlist_file)
{
    QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);

    QFileInfo pathInfo(PLAYLISTS_PATH);
    //if(!pathInfo.exists())
    //    return INVALID_DIR;

    settings.beginGroup("Playlists");
    int count = settings.value("Number",1).toInt();

    settings.setValue("Number", count+1);
    settings.setValue("Playlist"+QString::number(count), playlist_name);
    settings.setValue("File"+QString::number(count), QDir::fromNativeSeparators(playlist_file));

    settings.endGroup();
}

void SimplyWindow::DeleteEntryToPlaylistFile(QString playlist_name)
{
    QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);

    QFileInfo pathInfo(PLAYLISTS_PATH);
    //if(!pathInfo.exists())
    //    return INVALID_DIR;

    settings.beginGroup("Playlists");

    int count = settings.value("Number",1).toInt();
    int number = 0;

    for(int i = 0; i < count; i++)
    {
        if(settings.value("Playlist"+QString::number(i), "") == playlist_name)
        {
           number = i;
           break;
        }
    }

    settings.remove("Playlist"+QString::number(number));
    settings.remove("File"+QString::number(number));

    for(int j = number; j < count; j++)
    {
        settings.setValue("Playlist"+QString::number(j), settings.value("Playlist"+QString::number(j+1), ""));
        settings.setValue("File"+QString::number(j), settings.value("File"+QString::number(j+1), ""));
    }

    settings.remove("Playlist"+QString::number(count));
    settings.remove("File"+QString::number(count));

    settings.setValue("Number", count-1);


    settings.endGroup();
}

void SimplyWindow::removeItemFromPlaylist(int row)
{
   QTab *widget = (QTab *)(ui->tabWidget->currentWidget());
   QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();

    for(int i = 0; i < allTabs.count(); i++)
    {
        if(widget->GetTabName() == allTabs[i]->GetPlaylistName())
        {
            allTabs[i]->takeTopLevelItem(row);
            allTabs[i]->RefreshPlaylist();
        }

    }

}

void SimplyWindow::LabelTimerEvent()
{
    switch(state)
    {

    case STATE_CONNECTING:
    {
        ui->SongName->move(130,1);
        ui->SongName->setFixedWidth(90);

        QString cns = "Connecting";

        for(int i = 0; i < connecting_dots/30; i++)
            cns += ". ";

        connecting_dots = connecting_dots >= 90 ? 0 : connecting_dots+1;
        ui->SongName->setText(cns);
        break;
    }


    case STATE_NAME_1:
    {
        QPalette palette = ui->SongName->palette();
        QColor color = palette.color(QPalette::Text);
        //palette.setColor(QPalette::WindowText, QColor(qRgba(0, 0, 0, QString(percent).toInt())));
        ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba("+ QString::number(color.red()) +", "+ QString::number(color.green()) +", "+ QString::number(color.blue()) +", 100%);}");
        int wd = Player->GetLastFileName().length()*6;
        ui->SongName->setFixedWidth(wd);
        ui->SongName->setText(Player->GetLastFileName());
        if(Player->GetState() == 1)
        {
             ui->SongName->move(ui->SongName->x()-1,1);

             if(ui->SongName->x() <= 0-wd) //NAME_LENGTH)
                //ui->SongName->move(Player->GetLastFileName().length()*(6),NAME_LENGTH+wd);
                 ui->SongName->move(NAME_LENGTH+wd,0);
        }

        break;
    }

    case STATE_NAME_2:
    {

        if(percent < 100)
        {
           percent += 5;


        }
        else
        {
           percent = 100;
           state = STATE_NAME_1;
        }

        QPalette palette = ui->SongName->palette();
        QColor color = palette.color(QPalette::Text);
        //palette.setColor(QPalette::Text,QColor(qRgba(0, 0, 0, QString(percent).toInt())));
        //ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba(255, 255,255, " + QString::number(percent) +"%);}");
        ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba("+ QString::number(color.red()) +", "+ QString::number(color.green()) +", "+ QString::number(color.blue()) +", " + QString::number(percent) +"%);}");
        int wd = Player->GetLastFileName().length()*6;
        ui->SongName->setFixedWidth(wd);
        ui->SongName->setText(Player->GetLastFileName());
        if(Player->GetState() == 1)
        {
             ui->SongName->move(ui->SongName->x()-1,1);

            if(ui->SongName->x() <= 0-wd)
               // ui->SongName->move(Player->GetLastFileName().length()*(6),1);
                ui->SongName->move(NAME_LENGTH+wd,0);
        }

        break;
    }

    case STATE_VOLUME_1:
    {

        if(percent < 100)
        {
           percent += 5;

           if(percent > 100)
           {
               percent = 100;
           }

        }
        else
        {
           percent = 100;
        }
        QPalette palette = ui->SongName->palette();
        QColor color = palette.color(QPalette::Text);
        //palette.setColor(QPalette::WindowText, QColor(qRgba(0, 0, 0, QString(percent).toInt())));

        ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba("+ QString::number(color.red()) +", "+ QString::number(color.green()) +", "+ QString::number(color.blue()) +", " + QString::number(percent) +"%);}");
        ui->SongName->move(STATE_POS,1);
        ui->SongName->setText("Volume: " + QString::number(ui->Volume->value()) + "%");
        break;
    }

    case STATE_VOLUME_2:
    {

        if(percent > 0)
        {
           percent -= 5;

           if(percent < 0)
           {
               percent = 0;
           }

        }
        else
        {
            percent = 0;
            ui->SongName->move(1,1);
            state = STATE_NAME_2;
        }
        QPalette palette = ui->SongName->palette();
        QColor color = palette.color(QPalette::Text);
        //palette.setColor(QPalette::WindowText, QColor(qRgba(0, 0, 0, QString(percent).toInt())));
        //color.setAlpha(QString(percent).toInt());
        //palette.setColor(QPalette::Text, color);
        ui->SongName->setPalette(palette);
        ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba("+ QString::number(color.red()) +", "+ QString::number(color.green()) +", "+ QString::number(color.blue()) +", " + QString::number(percent) +"%);}");
        //ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba(255, 255,255, " + QString::number(percent) +"%);}");
        ui->SongName->move(STATE_POS,1);
        ui->SongName->setText("Volume: " + QString::number(ui->Volume->value()) + "%");
        break;
    }

    case STATE_BALANCE_1:
    {

        if(percent < 100)
        {
           percent += 5;

           if(percent > 100)
           {
               percent = 100;
           }

        }
        else
        {
           percent = 100;
        }
        QPalette palette = ui->SongName->palette();
        QColor color = palette.color(QPalette::Text);
        //palette.setColor(QPalette::WindowText, QColor(qRgba(0, 0, 0, QString(percent).toInt())));
        ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba("+ QString::number(color.red()) +", "+ QString::number(color.green()) +", "+ QString::number(color.blue()) +", " + QString::number(percent) +"%);}");
       // ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba(255, 255,255, " + QString::number(percent) +"%);}");
        ui->SongName->move(STATE_POS,1);
        ui->SongName->setText("Balance: " + QString::number(ui->Balance->value()));
        break;
    }

    case STATE_BALANCE_2:
    {

        if(percent > 0)
        {
           percent -= 5;

           if(percent < 0)
           {
               percent = 0;
           }

        }
        else
        {
            percent = 0;
            ui->SongName->move(1,1);
            state = STATE_NAME_2;
        }
        QPalette palette = ui->SongName->palette();
        QColor color = palette.color(QPalette::Text);
        //palette.setColor(QPalette::WindowText, QColor(qRgba(0, 0, 0, QString(percent).toInt())));

        ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba("+ QString::number(color.red()) +", "+ QString::number(color.green()) +", "+ QString::number(color.blue()) +", " + QString::number(percent) +"%);}");
        //ui->SongName->setStyleSheet("#centralWidget QLabel { color: rgba(255, 255,255, " + QString::number(percent) +"%);}");
        ui->SongName->move(STATE_POS,1);
        ui->SongName->setText("Balance: " + QString::number(ui->Balance->value()));
        break;
    }

  }


}

void SimplyWindow::resizeAlbumInfoWindow()
{

    ui->graphicsView->setFixedSize(albumInfo->width()-10,albumInfo->height()/2-5);
    //ui->textBrowser->setFixedSize(albumInfo->width()-12,albumInfo->height()/2-25);
    ui->textBrowser->setFixedSize(albumInfo->width()-12,ui->textBrowser->height());
    ui->textBrowser->move(ui->textBrowser->x(),albumInfo->height()-ui->textBrowser->height()-12);
    if(ui->graphicsView->scene() != 0)
        ui->graphicsView->fitInView(coveritem ,Qt::KeepAspectRatio);

}

void SimplyWindow::resizePlaylistWindow()
{

    ui->tabWidget->move(0,22);
    ui->tabWidget->setFixedSize(playlistDock->width(),playlistDock->height()-30);
    ui->statusBar->move(0,playlistDock->height()-32);
    ui->statusBar->setFixedSize(playlistDock->width()-20,ui->statusBar->height());
   // ui->tabWidget->move(0,0);

    if(afterInit)
    {
        QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();

        for(int i = 0; i < allTabs.count();i++)
        {
            allTabs[i]->setFixedSize(playlistDock->width()-20,playlistDock->height()-90);
            allTabs[i]->RefreshPlaylist(false);
        }
    }


}

void SimplyWindow::resizeEvent(QResizeEvent *e)
{
   /*short int mod = ui->widget_3->isExpanded() ? 100 : 0;

   ui->tabWidget->setFixedHeight(ui->centralWidget->height()-155-mod);

   ui->tabWidget->move(ui->tabWidget->x(),180+mod);

   //ui->findButton->setFixedHeight(ui->centralWidget->height()-180);
  // if(ui->tabWidget->currentWidget() != 0)
  // {
      // ui->widget_2->move(ui->widget_2->geometry().x(),ui->tabWidget->geometry().y()+ui->tabWidget->currentWidget()->height()+22);
     //  ui->findButton->move(ui->findButton->geometry().x(),ui->tabWidget->geometry().y()+ui->tabWidget->currentWidget()->height()+21);
      // ui->findEdit->move(ui->findEdit->geometry().x(),ui->tabWidget->geometry().y()+ui->tabWidget->currentWidget()->height()+22-mod);
  // }
   QList<QPlaylist *> allTabs = ui->centralWidget->findChildren<QPlaylist *>();

     for(int i = 0; i < allTabs.count();i++)
        allTabs[i]->setFixedHeight(ui->centralWidget->height()-220-mod);

    // Player_Skin->updateStyle();
    // Player_Skin->repaint();
*/





     QMainWindow::resizeEvent(e);
}


void SimplyWindow::ResizeTimerEvent()
{
    //QList<QPlaylist *> allTabs = ui->centralWidget->findChildren<QPlaylist *>();

    // for(int i =0; i < allTabs.count();i++)
    //     allTabs[i]->setFixedHeight(ui->centralWidget->height()-220);

}

void SimplyWindow::SpectrumTimerEvent()
{

}
void SimplyWindow::DrawSpectrum()
{

}


void SimplyWindow::barButtonClick()
{
   // if(myMenu.)
    //myMenu.
 //   if(!isBBclicked)
 //   {
   //     isBBclicked = true;

         mainMenu.exec(this->mapToGlobal(QPoint(0,17)));
   // }

   // isBBclicked = false;
}

void SimplyWindow::checkPlaylistWidget()
{

    playlistDock->setHidden(!playlista->isChecked());

    QSettings settings(CONFIG_PATH, QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("playlist_hidden", !playlista->isChecked());
    settings.endGroup();


}

void SimplyWindow::checkAlbumInfoWidget()
{

    albumInfo->setHidden(!albuminf->isChecked());

    QSettings settings(CONFIG_PATH, QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("albuminfo_hidden", !albuminf->isChecked());
    settings.endGroup();

}

void SimplyWindow::searchMusic()
{
    src->show();
    QRect scr = QApplication::desktop()->screenGeometry();
    src->move(scr.center() - rect().center());
}

void SimplyWindow::openSettings()
{
    settingsForm->show();
    QRect scr = QApplication::desktop()->screenGeometry();
    settingsForm->move(scr.center() - rect().center());
}


void SimplyWindow::PlaylistTimerEvent()
{

  if(bufferEnabled)
  {
    ui->buffered->setMaximum(Player->GetDataSize());
    ui->buffered->setValue(Player->GetBufferDataSize());
  }

  if(Player->GetState() == 1)
  {
    PTime Time = Player->GetCurrentPTime();

    //ui->minutes->display((int)Time.minutes);
   // ui->seconds->display(Time.seconds);

   // qDebug() << Player->GetBufferDataSize();

    ui->Panel->setText(QString(OneToTwo((int)Time.minutes) + ":" + OneToTwo(Time.seconds)));
    ui->trackbar->setEnabled(true);
    ui->trackbar->setMaximum(Player->GetPlayerTime());
    ui->trackbar->setValue(Player->GetPlayerCurrentTime());

    if((Player->GetPlayerCurrentTime() == Player->GetPlayerTime()) || nextIndex)
    {

        if(currentPlaylist->GetPlaylistRepeatIndex() == GlobalIndex)
        {
           // GlobalIndex = currentPlaylist->GetPlaylistRepeatIndex();
            Player->ChangePosition(0);
            return;
        }


        for(int j = 0; j < currentPlaylist->topLevelItemCount();j++)
        {

            if(currentPlaylist->topLevelItem(j)->textColor(1) == highlightColor)
            {
                for(int w = 1; w < 6; w++)
                    currentPlaylist->topLevelItem(j)->setTextColor(w,styleHighlightColor);
            }


        }

        for(int i = 1; i < 7;i++)
        {
           // if(!nextIndex)
            // currentPlaylist->topLevelItem(GlobalIndex)->setTextColor(i,styleHighlightColor);

            currentPlaylist->topLevelItem((GlobalIndex+1 >= currentPlaylist->topLevelItemCount()) ? 0 : GlobalIndex+1)->setTextColor(i,highlightColor);
        }


        nextIndex = false;
        //QList<QPlaylist *> Tab = ui->tabWidget->currentWidget()->findChildren<QPlaylist *>();
        if(currentPlaylist->GetPlaylistRepeatIndex() == GlobalIndex)
        {
            Player->Play();
            return;
        }

        Player->Stop();
        GlobalIndex++;

        if(GlobalIndex >= currentPlaylist->topLevelItemCount())
           GlobalIndex = 0;

        if(currentPlaylist->GetPlaylistRepeatIndex() != -1)
            GlobalIndex = currentPlaylist->GetPlaylistRepeatIndex();


            QString Pt = currentPlaylist->topLevelItem(GlobalIndex%currentPlaylist->topLevelItemCount())->data(3,0).toString();
            QFileInfo pathInfo(Pt);
           // QByteArray bytes = Pt.toLatin1();
           // file = bytes.data();
           // Player->LoadFile(file);

            if(SimplyFunctions::isHTTP(Pt))
            {
                if(SimplyFunctions::isWrzutaAd(Pt))
                {
                    /*QString a = ConvertWrzutaAddress(Pt);
                    FileDownloader *wrzuta = new FileDownloader(QUrl(a));

                    QEventLoop loop;
                    connect(wrzuta, SIGNAL(downloaded()), &loop, SLOT(quit()));
                    loop.exec();

                    Player->SetLastFileName(WrzutaD->getFName(wrzuta->downloadedData()));
                    Player->LoadFile(WrzutaD->getFileURL(wrzuta->downloadedData())); -g --max-quality=18 -f mp4
*/
                }
                else if(SimplyFunctions::isYoutubeAd(Pt))
                {
                   // QProcess youtube_dl;
                    youtube_dl = new QProcess();
                    youtube_dl->start( youtubedl_path, QStringList() << Pt << "-g" << "--max-quality=18" << "-f" << "mp4" );
                    youtube_temp_index = GlobalIndex%currentPlaylist->topLevelItemCount();
                    connect(youtube_dl, SIGNAL(finished(int)), this, SLOT(youtubedlProcessFinished(int)));
                    state = STATE_CONNECTING;
                    LabelTimer->start(LLTIME);
                   // ui->buffered->setEnabled(true);
                    bufferEnabled = true;
                    return;
                }
            }
            else
            {

                Player->LoadFile(Pt);
                ui->buffered->setValue(0);
               // ui->buffered->setEnabled(false);
                bufferEnabled = false;

            }

            Player->Play();
            Simply_Hint->show();
            QString name = currentPlaylist->topLevelItem(GlobalIndex%currentPlaylist->topLevelItemCount())->data(2,0).toString();
            Simply_Hint->Start(name,50);

    }
  }

}


void SimplyWindow::youtubedlDownloadFinished(int itemindex)
{
    qDebug() << "hoj";
 qDebug() << itemindex;
}


void SimplyWindow::youtubedlProcessFinished(int errorcode)
{
  //  if ( youtube_dl.waitForFinished(3000) )
  //  {
         //  qDebug() << youtube_dl.readAllStandardOutput() + "&title=file";
  //  if(SimplyFunctions::isHTTP(youtube_dl.readAllStandardOutput()))
  //  {

        for(int j = 0; j < currentPlaylist->topLevelItemCount(); j++)
            if(currentPlaylist->topLevelItem(j)->textColor(1) == highlightColor)
                youtube_temp_index = j;

        qDebug() << youtube_temp_index;
        if(SimplyFunctions::isYoutubeAd(currentPlaylist->topLevelItem(youtube_temp_index)->data(3,0).toString()))
        {

         Player->Stop();
         Player->SetLastFileName(currentPlaylist->topLevelItem(youtube_temp_index)->data(2,0).toString());
         Player->LoadFile(youtube_dl->readAllStandardOutput() + "&title=file");
         Player->Play();

         ui->Play->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Pause.qss")));
         playthumb->setIcon(QIcon(QDir::fromNativeSeparators(APPLICATION_PATH + "data/temp/thumbbuttons/Pause.png")));
         playthumb->setToolTip("Pause");

         Simply_Hint->show();
         QString name = currentPlaylist->topLevelItem(youtube_temp_index)->data(2,0).toString();
         Simply_Hint->Start(name,50);
         state = STATE_NAME_1;
         PlaylistTimer->start(PLTIME);
         LabelTimer->start(LLTIME);
         GlobalIndex = youtube_temp_index;


        }

         //youtube_dl.close();
       //  delete youtube_dl;


   // }
   // }
  //  else
  //  {
        // QMessageBox::about(0,"Youtube-dl not found.","This file requires youtube-dl application.\nSet correct path in settings.");
  //  }


}


void SimplyWindow::paintEvent(QPaintEvent *event)
{
   // Player_Skin->updateStyle();
    //Player_Skin->repaint();
   // Player_Skin->updateStyle();
    QWidget::paintEvent(event);
}

QString SimplyWindow::OneToTwo(int Org)
{
  if(Org < 10)
        return "0" + QString::number(Org);
  else
        return QString::number(Org);
}

bool SimplyWindow::IsSupportedFormat(QString format)
{
   /* for(int i = 0; i > supported_formats.count();i++)
    {
        if(format == supported_formats[i])
            return true;
    }

    return false;
*/
    return true;
}

void SimplyWindow::on_trackbar_sliderMoved(int position)
{

    Player->ChangePosition(position);
    PTime Time = Player->GetCurrentPTime();
    ui->Panel->setText(QString(OneToTwo((int)Time.minutes) + ":" + OneToTwo(Time.seconds)));
}


void SimplyWindow::on_Open_clicked()
{
    QString Path = QFileDialog::getOpenFileName(this, tr("Open Music File"),
                                                     "",
                                                     tr("Music Files (*.mp3 *.wav *.ogg)"));
    if(Path.count() > 1)
    {
        Player->LoadFile(Path);
        Player->Play();
        PlaylistTimer->start(PLTIME);
        //newthread->startTimer(LLTIME);
        LabelTimer->start(LLTIME);
        AddFileToPlaylist(Path,currentPlaylist);
        GlobalIndex = currentPlaylist->topLevelItemCount();
        //Player->DrawSpectrum(ui->Spectrum, 0, 0, 260, 31);
       // SpectrumTimer->start(1);
    }
}

void SimplyWindow::on_Volume_sliderMoved(int position)
{
    ui->Vollabel->setText("Volume: " + QString::number(position) + "%");
    Player->SetVolume(position);
}

void SimplyWindow::on_Balance_sliderMoved(int position)
{

    if(position < -9)
            ui->Ballabel->setText("Left: " + QString::number(((position*-1)*2)-20));

    else if(position > 9)
            ui->Ballabel->setText("Right: " + QString::number((position)*2-20));

    else if(position < 10 && position > -10)
             ui->Ballabel->setText("Center");

    Player->SetBalance(position);

}

void SimplyWindow::doubleClicked(QTreeWidgetItem * item, int column)
{
   // QString Path = ui->Playlist->currentItem()->data(3,0).toString();
    QString Path = item->data(3,0).toString();
    QByteArray bytes = Path.toLatin1();
    char *file = bytes.data();

    QList<QPlaylist *> playlist = ui->tabWidget->currentWidget()->findChildren<QPlaylist *>();
 // highlightColor

    for(int j = 0; j < playlist[0]->topLevelItemCount();j++)
    {
        if(playlist[0]->topLevelItem(j)->textColor(1) == highlightColor)
        {
            for(int w = 1;w < 6;w++)
                playlist[0]->topLevelItem(j)->setTextColor(w,styleHighlightColor);
        }

    }


    for(int i = 1; i < 6;i++)
    {
        //playlist[0]->topLevelItem(GlobalIndex)->setTextColor(i,styleHighlightColor);
        item->setTextColor(i,highlightColor);
    }


    if(Path.count() > 1)
    {
        QFileInfo pathInfo(Path);

        if(SimplyFunctions::isRTSP(Path))
        {
            Player->SetLastFileName("");
            Player->LoadFile(Path);
        }

        else if(SimplyFunctions::isHTTP(Path))
        {
            if(SimplyFunctions::isWrzutaAd(Path))
            {
                QString a = ConvertWrzutaAddress(Path);
                FileDownloader *wrzuta = new FileDownloader(QUrl(a));


                QEventLoop loop;
                connect(wrzuta, SIGNAL(downloaded()), &loop, SLOT(quit()));
                loop.exec();

                Player->SetLastFileName(WrzutaD->getFName(wrzuta->downloadedData()));
                Player->LoadFile(WrzutaD->getFileURL(wrzuta->downloadedData()));

            }
            else if(SimplyFunctions::isYoutubeAd(Path))
            {
               // QYouTubeParser *par = new QYouTubeParser(Path);

               // qDebug() << par->getTitle();

              //  downloadList.append(file);
                //connect(par, SIGNAL(parsed(QYouTubeParser*)), this, SLOT(playFileSlot(QYouTubeParser*)));

               // Player->SetLastFileName(par->getTitle());

                youtube_dl = new QProcess();
                youtube_dl->start( youtubedl_path, QStringList() << Path << "-g" << "--max-quality=18" << "-f" << "mp4" );
                youtube_temp_index = atoi(item->data(1,0).toString().toLatin1().data())-1;
                state = STATE_CONNECTING;
                LabelTimer->start(LLTIME);
                connect(youtube_dl, SIGNAL(finished(int)), this, SLOT(youtubedlProcessFinished(int)));
                return;



/*
                QList<YOUTUBE_FILE> list = par->getFileLinks();
                for(int i = 0; i < list.size(); i++)
                {
                    qDebug() << "Rozmiar listy:" << list.size();
                    qDebug() << "Link: " << list[i].fileURL;
                    if(Player->LoadFile(list[i].fileURL) == 0)
                        break;

                }
*/

            /*    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
                connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
                QNetworkRequest request(QUrl(Path));
                QNetworkReply *sentReply = manager->get(request);
                return;
              */
             //   qDebug() << Path;
             //   FileDownloader *fl = new FileDownloader();

               /* QThread thread;
                fl->moveToThread(&thread);
                fl->connect(&thread, SIGNAL(started()), SLOT(downloadFiles()));
                thread.connect(fl, SIGNAL(downloaded()), SLOT(quit()));

                thread.start();*/
             /*   //fl->download(QUrl(Path));
                DOWNLOAD_FILE *file = new DOWNLOAD_FILE();
                file->setFileDownloader(fl);
                file->setFileMode(1);

            //    file.file = fl;

            //    file.mode = 1;

                downloadList.append(file);
               // qDebug() << downloadList[0]->file->;


                if(!DownloadTimer->isActive())
                    DownloadTimer->start(100);
               */// qDebug() << "1";

            }
        }
        else if(SimplyFunctions::isRadio(Path))
        {
            Path.remove(0,6);
            QByteArray a = Path.toLatin1();
            char *asd = a.data();
            qDebug() << Path;
            Player->LoadFile(asd);

        }
        else
        {
            qDebug() << "Load file: " << Path;
            Player->LoadFile(Path);

        }

        Player->Play();
        //GlobalIndex = atoi(ui->Playlist->currentItem()->data(1,0).toString().toLatin1().data())-1;
        GlobalIndex = atoi(item->data(1,0).toString().toLatin1().data())-1;

        if(!LabelTimer->isActive())
            LabelTimer->start(LLTIME);
            //newthread->startTimer(LLTIME);

        if(!PlaylistTimer->isActive())
            PlaylistTimer->start(PLTIME);

        Simply_Hint->show();
        Simply_Hint->Start(item->data(2,0).toString(),50);

        //Player->DrawSpectrum(ui->frame_2, 0, 0, 260, 31);
        //SpectrumTimer->start(1);
        ui->Play->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Pause.qss")));
    }
    //delete file;
}

void SimplyWindow::on_Play_clicked()
{
    if(Player->GetState() == 1)
    {

        type = PAUSE;
        lower->start(100);
        last_volume = Player->GetVolume();
        volume = Player->GetVolume();
        ui->Play->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Play.qss")));
        playthumb->setIcon(QIcon(QDir::fromNativeSeparators(APPLICATION_PATH +"data/temp/thumbbuttons/Play.png")));
        playthumb->setToolTip("Play");



        //Player->Stop();
    }
    else if(Player->GetState() == 2)
    {
        Player->Play(); 
        ui->Play->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Pause.qss")));
        playthumb->setIcon(QIcon(QDir::fromNativeSeparators(APPLICATION_PATH + "data/temp/thumbbuttons/Pause.png")));
        playthumb->setToolTip("Pause");


    }
}
void SimplyWindow::PlayStop()
{

    on_Play_clicked();
}

void SimplyWindow::on_Pause_clicked()
{

   type = PAUSE;
   lower->start(100);
   last_volume = Player->GetVolume();
   volume = Player->GetVolume();
}

void SimplyWindow::on_Stop_clicked()
{

   type = STOP;
   lower->start(100);
   last_volume = Player->GetVolume();
   volume = Player->GetVolume();
}


void SimplyWindow::on_Forward_clicked()
{
  // Player->Forward(ui->trackbar);
    playlistDock->updateGeometry();
    playlistDock->update();
    playlistSkin->repaint();
    playlistSkin->update();
   // playlistSkin->getResizeHandler()->doResize();
   // playlistDock->resize(1000,1000);
    playlistDock->setGeometry(playlistDock->geometry());
    this->repaint();
    qDebug() << playlistDock->geometry();
    nextIndex = true;
}

void SimplyWindow::on_Backward_clicked()
{
   // QList<QPlaylist* > playlist = ui->tabWidget->currentWidget()->findChildren<QPlaylist*>();
    //for(int i = 1; i < 4;i++)
   //     playlist[0]->topLevelItem(GlobalIndex)->setTextColor(i,styleHighlightColor);

    //GlobalIndex -= 2;
    //nextIndex = true;
   // QString a = "http://www.youtube.com/watch?v=RCqEkWY8ekQ";

    //QYouTubeParser *par = new QYouTubeParser("http://www.youtube.com/watch?v=FEmqtkxiLCs");

   /* FileDownloader *wrzuta2 = new FileDownloader(QUrl(a));
    QEventLoop loop;
    connect(wrzuta2, SIGNAL(downloaded()), &loop, SLOT(quit()));
    loop.exec();


    QString content = wrzuta2->downloadedData();
    qDebug() << content;
*/   //Player->Backward(ui->trackbar);
     //   QList<YOUTUBE_FILE> a = par->getFileLinks();

  //  for(int i = 0; i < a.size(); i++)
  //      qDebug() << a[i].fileURL;

  //  QYouTubeParser *parser = new QYouTubeParser(Pt);
  //  QList<YOUTUBE_FILE> lnk = parser->getFileLinks();

 /*   if(lnk.count() > 0)
    {
        Player->SetLastFileName(parser->getTitle());
        Player->LoadFile(lnk[0].fileURL);
    }
   */



}
/*
void SimplyWindow::createEventTimer(FileDownloader *filedown, int event)
{
    switch(event)
    {
        case 1:
        {
            QTimer* timer = new QTimer(this);
            FileDownloader *file = filedown;
            signalMapper = new QSignalMapper(this);
            FILEDOWNLOAD_MAP map;
            map.d = file;
            map.time = timer;



            connect(timer,SIGNAL(timeout()),signalMapper,SLOT(map()));
            signalMapper->setMapping(timer,map);
            connect(signalMapper,SIGNAL(mapped(FILEDOWNLOAD_MAP)),this,SIGNAL(timeout(FILEDOWNLOAD_MAP)));

            connect(this,SIGNAL(timeout(FILEDOWNLOAD_MAP)),this,SLOT(showPageData(FILEDOWNLOAD_MAP)));
            timer->start(100);
        }
        break;
        default:break;

    }

}
*/

void SimplyWindow::downloadTimerEvent()
{



}
/*
void SimplyWindow::showPageData(FILEDOWNLOAD_MAP *map)
{

    if(map->d->isDownloaded)
    {
        qDebug() << map->d->downloadedData();
        map->time->stop();

    }

}*/


void SimplyWindow::on_Balance_sliderPressed()
{
   percent = 0;
   state = STATE_BALANCE_1;
}

void SimplyWindow::on_Balance_sliderReleased()
{
    percent = 100;
    state = STATE_BALANCE_2;
}

void SimplyWindow::on_Volume_sliderPressed()
{
   percent = 0;
   state = STATE_VOLUME_1;
}

void SimplyWindow::on_Volume_sliderReleased()
{
    percent = 100;
    state = STATE_VOLUME_2;
}

void SimplyWindow::onDeleteItem()
{
    //qDebug() << "DeleteItem clicked";
    QTab *widget = (QTab *)(ui->tabWidget->currentWidget());
    QList<QPlaylist *> playlist = widget->findChildren<QPlaylist *>();
   // qDebug() << "Found: " << playlist.count();
   // qDebug() << "Index of: " << playlist[0]->indexAt(QCursor::pos()).row();
   // qDebug() << "Global position x: " << QCursor::pos().x() << " y: " << QCursor::pos().y();
    QPoint p = playlist[0]->mapFromGlobal(QCursor::pos());
   // qDebug() << "Mapped position x: " << p.x() << " y: " << p.y();
    playlist[0]->takeTopLevelItem(playlist[0]->indexAt(p).row());


}

void SimplyWindow::setRepeat()
{
    QTab *widget = (QTab *)(ui->tabWidget->currentWidget());
    QList<QPlaylist *> playlist = widget->findChildren<QPlaylist *>();
    QPoint p = playlist[0]->mapFromGlobal(QCursor::pos());




    if(playlist[0]->GetPlaylistRepeatIndex() > -1)
    {
        if(playlist[0]->GetPlaylistRepeatIndex() == playlist[0]->indexAt(p).row())
        {
            playlist[0]->itemWidget(playlist[0]->itemAt(p),6)->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItem.qss")));
            playlist[0]->SetPlaylistRepeatIndex(-1);
        }
        else
        {
            playlist[0]->itemWidget(playlist[0]->topLevelItem(playlist[0]->GetPlaylistRepeatIndex()),6)->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItem.qss")));
            playlist[0]->SetPlaylistRepeatIndex(playlist[0]->indexAt(p).row());
            playlist[0]->itemWidget(playlist[0]->itemAt(p),6)->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItemSet.qss")));
        }
    }
    else
    {
        playlist[0]->SetPlaylistRepeatIndex(playlist[0]->indexAt(p).row());
        playlist[0]->itemWidget(playlist[0]->itemAt(p),6)->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItemSet.qss")));
    }


}

void SimplyWindow::SetPLH(int h)
{
    //ui->Playlist->setFixedHeight(ui->Playlist->height()+h);
}

void SetGlobalIndex(int index)
{
   GlobalIndex = index;
}

int GetGlobalIndex()
{
   return GlobalIndex;
}

void SimplyWindow::closeEvent(QCloseEvent * event)
{

  SavePlayerSettings();
  SavePlaylists();
  type = EXIT2;
  last_volume = Player->GetVolume();
  volume = Player->GetVolume();
  lower->start(100);


}
void SimplyWindow::CreateMirrorClock()
{

  // QGraphicsBlurEffect* effect	= new QGraphicsBlurEffect();
  // effect->setBlurRadius(2);
    //effect->setBlurRadius(3);
    //effect->setColor(Qt::white);
   // effect->setXOffset(15);
   // effect->setYOffset(15);
  // ui->Panel->setGraphicsEffect(effect);
   // this->setGraphicsEffect(effect);

}

void SimplyWindow::SavePlaylists()
{

    QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();


    for(int i = 0; i < allTabs.count();i++)
    {
        //QMessageBox::about(0,QString("543"),allTabs[i]->GetPlaylistPath());
        SavePLS(allTabs[i]->GetPlaylistPath(), allTabs[i]);
       // SavePlaylist(ui->tabWidget->tabText(i), QString(PLAYLISTD_PATH) + ui->tabWidget->tabText(i),i);
    }


}

void SimplyWindow::LowerTimerEvent()
{
    volume-=33;
    Player->SetVolume(volume);

    if(Player->GetVolume() <= 0)
    {
        if(type == STOP)
            Player->Stop();
        else if(type == PAUSE)
            Player->Pause();
        else
            this->close();

        Player->Pause();
        Player->SetVolume(last_volume);
        lower->stop();


    }

}

void SimplyWindow::on_trackbar_sliderPressed()
{
playlistDock->raise();

}

void SimplyWindow::onClickTrackBar()
{
    ui->trackbar->setValue(QCursor::pos().x());
    QMessageBox::about(0,tr("f"),tr("f"));
}


void SimplyWindow::SignalsTimerEvent()
{
    QFileInfo pathInfo(SIGNALS_PATH);

    if(pathInfo.size() > 0)
    {
        QSettings s(SIGNALS_PATH, QSettings::IniFormat);
        s.beginGroup("Signal");
        QString name = s.value("Name","0").toString();

        if(name == "CreateNewPlaylist")
        {
            this->addNewPlaylist(s.value("Arg0","0").toString(),s.value("Arg1","0").toString());
        }

        else if(name == "AddFileToCurrentPlaylist")
        {
            AddFileToPlaylist(s.value("Arg0","0").toString(), currentPlaylist, s.value("Arg1","").toString(), s.value("Arg2","").toString());
        }

        else if(name == "PlayFile")
        {
            Player->LoadFile(s.value("Arg0","0").toString());
            Player->Play();
            PlaylistTimer->start(PLTIME);
            //newthread->startTimer(LLTIME);
            LabelTimer->start(LLTIME);
            AddFileToPlaylist(s.value("Arg0","0").toString(),currentPlaylist);
            GlobalIndex = currentPlaylist->topLevelItemCount();
            Player->DrawSpectrum(ui->Spectrum, 0, 0, 260, 31);
        }

        else if(name == "SetActivePlaylist")
        {

            QList<QPlaylist *> playlists = playlistDock->findChildren<QPlaylist *>();

            for(int j = 0; j < playlists.count(); j++)
            {
                if(s.value("Arg0",currentPlaylist->GetPlaylistName()).toString() == playlists[j]->GetPlaylistName())
                {
                    currentPlaylist = playlists[j];
                    break;
                }
            }

        }

        else if(name == "PlayPause")
        {
                this->on_Play_clicked();
        }
        else if(name == "Forward")
        {
                this->on_Forward_clicked();
        }
        else if(name == "Backward")
        {
                this->on_Backward_clicked();
        }
        else if(name == "Stop")
        {
                this->on_Stop_clicked();
        }

        else if(name == "AddDelegatesToPlaylist")
        {


            QList<QPlaylist *> playlists = playlistDock->findChildren<QPlaylist *>();

            for(int j = 0; j < playlists.count(); j++)
            {
                if(s.value("Arg0",currentPlaylist->GetPlaylistName()).toString() == playlists[j]->GetPlaylistName())
                {
                 //   qDebug() << "Found: " << playlists[j]->GetPlaylistName();
                 //   qDebug() << "Arg0: " << s.value("Arg0",0).toInt();
                 //   qDebug() << "Arg1: " << s.value("Arg1",0).toInt();
                  /*  QPushButton *button1 = new QPushButton();
                    QPushButton *button2 = new QPushButton();
                    button1->setMaximumSize(20,15);
                    button2->setMaximumSize(20,15);
                    button1->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("DeleteDelegateItem.qss")));
                    button2->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItem.qss")));

                    connect(button1, SIGNAL(clicked()), this, SLOT(onDeleteItem()));
                    connect(button2, SIGNAL(clicked()), this, SLOT(setRepeat()));
                    //playlists[j]->item
                    */
                    QPushButton *delegateDelete = new QPushButton();
                    QPushButton *delegateRepeat = new QPushButton();
                    delegateDelete->setMaximumSize(20,15);
                    delegateRepeat->setMaximumSize(20,15);
                    delegateDelete->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("DeleteDelegateItem.qss")));
                    delegateRepeat->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("RepeatDelegateItem.qss")));

                    connect(delegateDelete, SIGNAL(clicked()), this, SLOT(onDeleteItem()));
                    connect(delegateRepeat, SIGNAL(clicked()), this, SLOT(setRepeat()));

                    playlists[j]->setItemWidget(playlists[j]->topLevelItem(s.value("Arg1",0).toInt()),5,delegateDelete);
                    playlists[j]->setItemWidget(playlists[j]->topLevelItem(s.value("Arg1",0).toInt()),6,delegateRepeat);

                    playlists[j]->RefreshPlaylist();
                    break;
                }
            }

        }


        s.endGroup();
        QFile file(SIGNALS_PATH);
        file.open(QFile::WriteOnly|QFile::Truncate);
        file.resize(0);
        file.close();

    }

    if(useGlassEffect)
        this->repaint();


}

void SimplyWindow::onCloseTab(int index)
{
    if(ui->tabWidget->count() == 1)
    {
        QMessageBox::question(0,"Deleting playlist","You cannot delete your last playlist.",QMessageBox::Ok);
    }
    else if(QMessageBox::Yes == QMessageBox::question(0,"Deleting playlist","Are you sure?",QMessageBox::Yes,QMessageBox::No))
    {
        QTab *widget = (QTab *)(ui->tabWidget->widget(index));
        DeleteEntryToPlaylistFile(widget->GetTabName());
        delete widget;
    }
}
void SimplyWindow::wrzutaPlay(QString data, int mode)
{

 /*   QString Pt = data;
    QString a = ConvertWrzutaAddress(Pt);
    Player->SetLastFileName(WrzutaD->getFName(data));
    Player->LoadFile(WrzutaD->getFileURL(data));
    Player->Play();
*/

    qDebug() << data << mode;

}

void SimplyWindow::youtubePlay(QString data, int mode)
{

  /*  QString Pt = data;
    QString a = ConvertWrzutaAddress(Pt);
    Player->SetLastFileName(WrzutaD->getFName(wrzuta->downloadedData()));
    Player->LoadFile(WrzutaD->getFileURL(wrzuta->downloadedData()));
    Player->Play();
*/
}


void SimplyWindow::keyPressEvent(QKeyEvent *k)
{


    switch(k->key())
    {

        case Qt::Key_Return:
        {
        QList<QPlaylist *> playlists = ui->tabWidget->currentWidget()->findChildren<QPlaylist *>();

        if(playlists[0]->selectedItems().count() > 0)
        {


            QString Pt = playlists[0]->topLevelItem(playlists[0]->currentIndex().row())->data(3,0).toString();

            if(Pt.size() > 0)
            {

                QByteArray bytes = Pt.toLatin1();
                char *file = bytes.data();
                SetGlobalIndex(playlists[0]->topLevelItem(playlists[0]->currentIndex().row())->data(1,0).toInt(0)-1);
                if(!LabelTimer->isActive())
                    LabelTimer->start(LLTIME);
                    //newthread->startTimer(LLTIME);

                if(!PlaylistTimer->isActive())
                    PlaylistTimer->start(PLTIME);

                Simply_Hint->show();
                Simply_Hint->Start(playlists[0]->topLevelItem(playlists[0]->currentIndex().row())->data(2,0).toString(),50);

                if(SimplyFunctions::isHTTP(Pt))
                {
                    if(SimplyFunctions::isWrzutaAd(Pt))
                    {
                        QString a = ConvertWrzutaAddress(Pt);
                        FileDownloader *wrzuta = new FileDownloader(QUrl(a));


                      //  QEventLoop loop;
                      //  connect(wrzuta, SIGNAL(downloaded()), &loop, SLOT(quit()));
                       // loop.exec();

                        connect(wrzuta, SIGNAL(downloaded(QString, int)), this, SLOT(wrzutaPlay(QString, int)));

                        return;
                        Player->SetLastFileName(WrzutaD->getFName(wrzuta->downloadedData()));
                        Player->LoadFile(WrzutaD->getFileURL(wrzuta->downloadedData()));

                    }
                    else if(SimplyFunctions::isYoutubeAd(Pt))
                    {
                        /*
                        QProcess youtube_dl;
                        youtube_dl.start( youtubedl_path, QStringList() << Pt << "-g" << "-f" << "mp4" );

                        youtube_temp_index = atoi(item->data(1,0).toString().toLatin1().data())-1;
                        connect(&youtube_dl, SIGNAL(finished(int)), this, SLOT(youtubedlProcessFinished(int)));
                        return;*/
                    }

                }
                else
                {


                    Player->LoadFile(file);

                }

              //  Player->LoadFile(file);
                Player->Play();
                Player->DrawSpectrum(ui->Spectrum, 0, 0, 260, 31);
              //  delete file;
            }

        }

           break;
        }

        case Qt::Key_Delete:
        {
            QList<QPlaylist *> playlists = ui->tabWidget->currentWidget()->findChildren<QPlaylist *>();

           if(playlists[0]->selectedItems().count() > 0)
           {
               playlists[0]->takeTopLevelItem(playlists[0]->topLevelItem(playlists[0]->currentIndex().row())->data(1,0).toInt(0)-1);
               playlists[0]->RefreshPlaylist();
           }

            break;
        }

        case Qt::Key_Up:
        {
            QList<QPlaylist *> playlists = ui->tabWidget->currentWidget()->findChildren<QPlaylist *>();

            if(playlists[0]->currentIndex().row()-1 >= 0)
            {
                 playlists[0]->currentItem()->setSelected(false);
                 playlists[0]->setItemSelected(playlists[0]->itemAbove(playlists[0]->currentItem()),true);
                 playlists[0]->setCurrentItem(playlists[0]->itemAbove(playlists[0]->currentItem()),0,QItemSelectionModel::NoUpdate);
            }

            break;
        }

        case Qt::Key_Down:
        {
            QList<QPlaylist *> playlists = ui->tabWidget->currentWidget()->findChildren<QPlaylist *>();

            if(playlists[0]->currentIndex().row() != playlists[0]->topLevelItemCount()-1)
            {
                playlists[0]->currentItem()->setSelected(false);
                playlists[0]->setItemSelected(playlists[0]->itemBelow(playlists[0]->currentItem()),true);
                playlists[0]->setCurrentItem(playlists[0]->itemBelow(playlists[0]->currentItem()),0,QItemSelectionModel::NoUpdate);
            }

            break;
        }

        case Qt::Key_Left:
        {

            Player->ChangePosition(Player->GetPlayerCurrentTime()-5);
            break;
        }

        case Qt::Key_Right:
        {

            Player->ChangePosition(Player->GetPlayerCurrentTime()+5);
            break;
        }


    default:
        break;

    }
 //   QPaintEvent *a;
    //Player_Skin->updateStyle();
  //  QMainWindow::keyPressEvent(k);

}

void SimplyWindow::on_findEdit_textChanged(const QString &arg1)
{
    QString search = ui->findEdit->text();

    QList<QPlaylist* > playlist = ui->tabWidget->currentWidget()->findChildren<QPlaylist*>();

    for(int i = 0; i < playlist[0]->topLevelItemCount();i++)
    {
        if(playlist[0]->topLevelItem(i)->text(3).contains(search, Qt::CaseInsensitive))
            playlist[0]->topLevelItem(i)->setHidden(false);
        else
            playlist[0]->topLevelItem(i)->setHidden(true);


    }
}

void SimplyWindow::on_tabWidget_currentChanged(int index)
{
    //QImage
    //qDebug() << "wetwert";
    QList<QPlaylist* > playlist = ui->tabWidget->currentWidget()->findChildren<QPlaylist*>();

    if(playlist.count() > 0)
    {

    QSettings settings(QDir::toNativeSeparators(playlist[0]->GetPlaylistPath()), QSettings::IniFormat);

    settings.beginGroup("playlist");

   // QImage image;
    //QDir::
    QPixmap pix = QPixmap(QDir::toNativeSeparators(settings.value("Cover",QString("covers/default.png")).toString()));

    if(pix.isNull())
        pix = QPixmap(QDir::toNativeSeparators(PLAYLISTD_PATH+"covers/default.png"));


    coveritem = new QGraphicsPixmapItem(pix);

    QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
    //qDebug() << QDir::toNativeSeparators(PLAYLISTD_PATH+"covers/default.jpg");
    //QGraphicsPixmapItem item(QPixmap::fromImage(image));
    //scene->addPixmap(pix);
    scene->addItem(coveritem);
    //scene->addItem(&item);
    //scene.
    //scene->addPixmap();
    ui->graphicsView->setScene(scene);
   // coveritem->setPos(0,0);
    ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(50,50,50), Qt::SolidPattern));
   // ui->graphicsView->setFrameShape(QFrame::);
    ui->graphicsView->show();
    ui->textBrowser->setText(settings.value("Description",QString("")).toString());

    settings.endGroup();

    playlist[0]->RefreshPlaylist(true);

    }
}


void SimplyWindow::closeMainWindow()
{
    this->SavePlayerSettings();
    this->SavePlaylists();
    QApplication::closeAllWindows();
}

void SimplyWindow::minMainWindow()
{
    this->showMinimized();
    playlistDock->hide();
    albumInfo->hide();
}

void SimplyWindow::maxMainWindow()
{

}

void SimplyWindow::closeAlbumInfoWindow()
{
    albumInfo->setHidden(true);

    QSettings settings(CONFIG_PATH, QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("albuminfo_hidden", true);
    settings.endGroup();
    albuminf->setChecked(false);

}
void SimplyWindow::closePlaylistWindow()
{
    playlistDock->setHidden(true);

    QSettings settings(CONFIG_PATH, QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("playlist_hidden", true);
    settings.endGroup();
    playlista->setChecked(false);

}

void SimplyWindow::minPlaylistWindow()
{
    playlistDock->showMinimized();
}

void SimplyWindow::maxPlaylistWindow()
{
    playlistDock->resize(playlistDock->width(),25);
    playlistDock->setFixedSize(playlistDock->size());
   // playlistDock->resize
}

void SimplyWindow::moveAlbumInfoWindow()
{
    if(afterInit)
    {
        if(albumInfo->isDocked() && albumInfo->isDockedDown())
        {
           // albuminfoSkin->setDockBorder(true);
            //albuminfoSkin->setFrameAlpha(this->windowOpacity());
           // albuminfoSkin->setFrameAlpha(alphaAI);
            playlistSkin->setFrameAlpha(0.8);
          //  albuminfoSkin->repaint();
        }
        else
        {
         //   albuminfoSkin->setFrameAlpha(alphaAI);
            playlistSkin->setFrameAlpha(0.8);
           // albuminfoSkin->setDockBorder(false);
           // albuminfoSkin->repaint();
        }
    }

}

void SimplyWindow::movePlaylistWindow()
{
    if(afterInit)
    {
        if(playlistDock->isDocked() && playlistDock->isDockedDown())
        {
           // playlistSkin->setDockBorder(true);
           // playlistSkin->setFrameAlpha(this->windowOpacity());
          //  playlistSkin->setFrameAlpha(alphaPL);
            playlistSkin->setFrameAlpha(0.8);
          //  playlistSkin->repaint();
        }
        else
        {
           // playlistSkin->setFrameAlpha(alphaPL);
           // playlistSkin->setDockBorder(false);
            playlistSkin->setFrameAlpha(0.8);
           // playlistSkin->repaint();
        }
    }

}

void SimplyWindow::moveEvent(QMoveEvent * event)
{

    int x = event->oldPos().x() - event->pos().x();
    int y = event->oldPos().y() - event->pos().y();
    QPoint PP = QPoint(playlistDock->x()-x,playlistDock->y()-y);
    QPoint AP = QPoint(albumInfo->x()-x,albumInfo->y()-y);

  //  qDebug() << "MainWindow: OX: " << event->oldPos().x() << " OY: " << event->oldPos().y() <<" X: " << event->pos().x() << " Y: " << event->pos().y();
  //  qDebug() << "AP: " << "Old: " << albumInfo->x() << albumInfo->y() << " New: " << AP.x() << AP.y();


    if((event->pos().x() == albumInfo->oldPos().x()) && (event->pos().y() == albumInfo->oldPos().y()))
    {
         albumInfo->setOldPos(event->pos().x(),event->pos().y());
         return;
    }
    else
         albumInfo->setOldPos(event->pos().x(),event->pos().y());

    if(afterInit)
    {

       // albumInfo->move(albumInfo->x(),albumInfo->y());
      //  playlistDock->move(playlistDock->x(),playlistDock->y());

        if((albumInfo->dockedTo() == this))
        {
            albumInfo->moveF(AP,false);
        }

        if(playlistDock->dockedTo() == this)
        {
            playlistDock->moveF(PP,false);
        }

        if(playlistDock->dockedTo() == albumInfo)
        {
            playlistDock->moveF(PP,false);
        }

        if(albumInfo->dockedTo() == playlistDock)
        {
            albumInfo->moveF(AP,false);
        }

    }

    /*
    if(albumInfo->isDocked() && playlistDock->isDocked())
    {
        playlistDock->move(PP);
        albumInfo->move(AP);
        playlistDock->setDocked(true);
        albumInfo->setDocked(true);

    }

    else if(albumInfo->isDocked())
        albumInfo->move(AP);


    else if(playlistDock->isDocked())
        playlistDock->move(PP);
*/

}


void SimplyWindow::changeEvent( QEvent* e )
{

    if( e->type() == QEvent::WindowStateChange )
    {
        QWindowStateChangeEvent* event = static_cast< QWindowStateChangeEvent* >( e );

        if( event->oldState() & Qt::WindowMinimized )
        {
           // qDebug() << "Window restored (to normal or maximized state)!";
            QSettings sett(CONFIG_PATH, QSettings::IniFormat);//("RavanBladeSoftware", "SimplyPlayer");
            sett.beginGroup("MainWindow");
            bool isPlaylistHid = sett.value("playlist_hidden", false).toBool();
            bool isAlbumInfoHid = sett.value("albuminfo_hidden", false).toBool();
            sett.endGroup();

            if(!isPlaylistHid)
            {
                playlistDock->raise();
                playlistDock->show();
            }
            if(!isAlbumInfoHid)
            {
                albumInfo->raise();
                albumInfo->show();
            }
        }
        else if( event->oldState() == Qt::WindowNoState && this->windowState() == Qt::WindowMaximized )
        {
            //qDebug() << "Window Maximized!";
        }

    }
    QMainWindow::changeEvent(e);
}


void SimplyWindow::doubleClickCover()
{
    QString Path = QFileDialog::getOpenFileName(this, tr("Replace cover"),
                                                     "",
                                                     tr("Image files (*.png *.bmp *.jpg)"));
    if(Path.count() > 1)
    {

        QSettings settings(QDir::toNativeSeparators(currentPlaylist->GetPlaylistPath()), QSettings::IniFormat);

        settings.beginGroup("playlist");

       // QImage image;
        //QDir::
        QPixmap pix = QPixmap(QDir::toNativeSeparators(Path));

        if(pix.isNull())
            pix = QPixmap(QDir::toNativeSeparators(PLAYLISTD_PATH+"covers/default.png"));
        else
            settings.setValue("Cover",Path);

        coveritem = new QGraphicsPixmapItem(pix);

        QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
        scene->clear();
        scene->addItem(coveritem);

        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);
        ui->graphicsView->setBackgroundBrush(QBrush(QColor(50,50,50), Qt::SolidPattern));
        ui->graphicsView->show();

        settings.endGroup();



    }


}


void SimplyWindow::on_textBrowser_textChanged()
{

}

void SimplyWindow::showEvent(QShowEvent * event)
{

    albumInfo->show();
    playlistDock->show();
    playlistDock->raise();
  //  playlistDock->setFocusPolicy(Qt::StrongFocus);
   // albumInfo->setAttribute(Qt::WA_ShowWithoutActivating);
  //  playlistDock->setAttribute(Qt::WA_ShowWithoutActivating);

    QWidget::showEvent(event);


}

void SimplyWindow::deletePlaylistButton()
{
    int index = ui->tabWidget->currentIndex();

    if(ui->tabWidget->count() == 1)
    {
        QMessageBox::question(0,"Deleting playlist","You cannot delete your last playlist.",QMessageBox::Ok);
    }
    else if(QMessageBox::Yes == QMessageBox::question(0,"Deleting playlist","Are you sure?",QMessageBox::Yes,QMessageBox::No))
    {
        QTab *widget = (QTab *)(ui->tabWidget->widget(index));
        DeleteEntryToPlaylistFile(widget->GetTabName());
        delete widget;
    }

}

void SimplyWindow::addNewPlaylistButton()
{
    newplaylistSkin->setLabelName("Create New Playlist");
    crpl->showW(0);

}

void SimplyWindow::editPlaylistButton()
{
    newplaylistSkin->setLabelName("Edit Playlist");

    QTab *widget = (QTab *)(ui->tabWidget->currentWidget());
    QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();

     for(int i = 0; i < allTabs.count(); i++)
     {
         if(widget->GetTabName() == allTabs[i]->GetPlaylistName())
         {
            crpl->showW(allTabs[i],widget);
            break;

         }
     }



}

void SimplyWindow::focusInEvent(QFocusEvent * event)
{

    QSettings sett(CONFIG_PATH, QSettings::IniFormat);//("RavanBladeSoftware", "SimplyPlayer");
    sett.beginGroup("MainWindow");
    bool isPlaylistHid = sett.value("playlist_hidden", false).toBool();
    bool isAlbumInfoHid = sett.value("albuminfo_hidden", false).toBool();
    sett.endGroup();

    if(!isPlaylistHid)
    {
        playlistDock->raise();
        playlistDock->show();
    }
    if(!isAlbumInfoHid)
    {
        albumInfo->raise();
        albumInfo->show();
    }

}

void SimplyWindow::createNewPlaylist(QString name, QString author, QString coverpath)
{

    QSettings settings("data//playlists//"+name+".pls", QSettings::IniFormat);

    settings.beginGroup("playlist");

    settings.setValue("NumberOfEntries", 0);
    settings.setValue("Description", author);


    settings.endGroup();

    addNewPlaylist(name,"data//playlists//"+name+".pls");
    AddEntryToPlaylistFile(name, "data//playlists//"+name+".pls");
    //SavePLS()


    QList<QPlaylist *> allTabs = playlistDock->findChildren<QPlaylist *>();
    int indx = -1;

    for(int i = 0; i < allTabs.count(); i++)
         if(name == allTabs[i]->GetPlaylistName())
            indx = i;

    QSettings set(QDir::toNativeSeparators(allTabs[indx]->GetPlaylistPath()), QSettings::IniFormat);

    set.beginGroup("playlist");

    QPixmap pix = QPixmap(QDir::toNativeSeparators(coverpath));

    if(!pix.isNull())
        set.setValue("Cover",coverpath);

    set.endGroup();


}

void SimplyWindow::reloadPlaylist(QString name, QString path, QPlaylist *playlist, bool isAct)
{

  SavePLS(path,playlist);
  int ind = ui->tabWidget->currentIndex();

  ui->tabWidget->removeTab(ind);
  int count = ui->tabWidget->count();

  QTab *tab;

  if(count == 0)
  {
     tab = new QTab(ui->tabWidget);
     ui->tabWidget->insertTab(0,tab, "temp");
  }

  QTab *newTab = new QTab(ui->tabWidget);
  if(count == 0)
    ui->tabWidget->addTab(newTab,name);
  else
    ui->tabWidget->insertTab(ind, newTab, name);
 // ui->tabWidget->insertTab(ind, newTab, name);
  QPlaylist *newpl = new QPlaylist(newTab);
  QTreeWidgetItem *itemo = new QTreeWidgetItem;
  itemo->setText(0,tr("1"));
  itemo->setText(1,tr("2"));
  itemo->setText(3,tr("3"));
  itemo->setText(4,tr("4"));
  itemo->setText(5,tr("5"));
  itemo->setText(6,tr("6"));
  newpl->setHeaderItem(itemo);
  newpl->setColumnWidth(0,0);
  newpl->setColumnWidth(2,180); // 200
  newpl->setColumnWidth(3,0);
  newpl->setColumnWidth(4,20);
  newpl->setColumnWidth(1,15);
  newpl->setColumnWidth(5,10);
  newpl->setColumnWidth(6,10);

  newpl->setMouseTracking(true);
  newpl->SetPlaylistName(name);
  newpl->SetPlaylistPath(path);
  newTab->SetTabName(name);
  newpl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  newpl->setFont(playlist_font);
  newpl->setAcceptDrops(true);
  newpl->setDragEnabled(true);
  newpl->setGeometry(9,9,ui->tabWidget->width()-20,ui->tabWidget->height()-60);
  newpl->setHeaderHidden(true);
  newpl->setAttribute(Qt::WA_Hover);
  newpl->setFrameShape(QFrame::NoFrame);
  newpl->setFrameShadow(QFrame::Plain);
  connect(newpl, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(doubleClicked(QTreeWidgetItem*, int)));
  newpl->setUniformRowHeights(true);
  newpl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
  newpl->setFocusPolicy(Qt::NoFocus);
  newpl->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Playlist.qss")));

  LoadPLS(path,newpl);

  if(count == 0)
  {
     ui->tabWidget->removeTab(0);
     //tab->des
     delete tab;
  }

  if(isAct)
  {
      newTab->SetActiveTab(true);
      QLabel *button1 = new QLabel();
      button1->setText(tr("    ") + ui->tabWidget->tabBar()->tabText(ind));
      button1->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("ActiveTab.qss")));
      ui->tabWidget->tabBar()->setTabButton(ind, QTabBar::LeftSide,((QWidget*)(button1)));
      ui->tabWidget->tabBar()->setTabText(ind,"");
      ui->tabWidget->tabBar()->tabButton(ind,QTabBar::LeftSide)->move(ui->tabWidget->tabBar()->tabButton(ind,QTabBar::LeftSide)->geometry().x(),ui->tabWidget->tabBar()->tabButton(ind,QTabBar::LeftSide)->geometry().y());

  }

  ui->tabWidget->setCurrentIndex(ind);

}

















