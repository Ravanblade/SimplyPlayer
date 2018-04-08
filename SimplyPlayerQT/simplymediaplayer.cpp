#include "simplymediaplayer.h"

SimplyMediaPlayer::SimplyMediaPlayer(QWidget *parent) :
    QThread(parent)
{
    MainChannel = NULL;
    isSpectrumStarted = false;
    LastVolume = -256;
    LastBalance = -256;
    stopThread = false;
    qDebug() << "Scasdasd";

    Init();
    start();
}

SimplyMediaPlayer::~SimplyMediaPlayer()
{

}

void SimplyMediaPlayer::run()
{


}

int SimplyMediaPlayer::Init()
{
    if(!BASS_Init(-1,44100,0,0,0))
    {
      return 0;
    }
    BASS_SetConfig(BASS_CONFIG_FLOATDSP,TRUE);
    //BASS_SFX_Init(0, 0);
    //hSFX1 = BASS_SFX_PluginCreate(/*"vis_classic.dll"*/"", 0, 0, 0, 0);
    //this->start();
    return 1;
}

bool SimplyMediaPlayer::isHTTPS(QString link)
{
    QStringList spl = link.split(":");

    return (spl[0] == "http") || (spl[0] == "https");
}

void SimplyMediaPlayer::LoadPlugin(QString file)
{
   QByteArray bytes = file.toLatin1();
   char *fl = bytes.data();
   BASS_PluginLoad(fl, 0);
}

int SimplyMediaPlayer::Play()
{
    if(!BASS_ChannelPlay(MainChannel,FALSE))
    {
        return 0;
    }

    return 1;
}

int SimplyMediaPlayer::LoadFile(QString file)
{

    short int error_code = 0;
    QByteArray bytes = file.toLatin1();
    filename = bytes.data();

    QFileInfo pathInfo(file);
    QUrl url(file);

    //qDebug() << file;
    if(!(url.isValid() && isHTTPS(file)))
    {

        LastPath = file;
        LastFileName = pathInfo.fileName();
    }



    if(MainChannel == 0 || BASS_ChannelIsActive(MainChannel) == BASS_ACTIVE_PLAYING)
    {
       BASS_ChannelStop(MainChannel);
    }

    BASS_MusicFree(MainChannel);
    BASS_StreamFree(MainChannel);


    if((isHTTPS(file)))
    {
        qDebug() << file;
        if(MainChannel = BASS_StreamCreateURL(filename, 0, 0, 0, 0))
        {

        }

        qDebug() << "Error code: " << BASS_ErrorGetCode();
    }
    else
    {
        if(!(MainChannel = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_SAMPLE_LOOP))
                && !(MainChannel = BASS_MusicLoad(FALSE,filename,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP,0)))
        {
          //  qDebug() << "hha";
            BASS_ChannelStop(MainChannel);
            BASS_MusicFree(MainChannel);
            BASS_StreamFree(MainChannel);
            if(!(MainChannel = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_SAMPLE_LOOP))
                    && !(MainChannel = BASS_MusicLoad(FALSE,filename,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP,0)))
            {/*qDebug() << "hauu";*/}

        }
    }
    error_code = BASS_ErrorGetCode();
    if(error_code == 0)
        qDebug() << file;

    if(LastVolume > -256 || LastBalance > -256)
    {
        BASS_ChannelSetAttribute(MainChannel, BASS_ATTRIB_VOL, (float)(LastVolume)/100.0f);
        BASS_ChannelSetAttribute(MainChannel, BASS_ATTRIB_PAN, (float)(LastBalance)/100.0f);
    }

    return error_code;

}

int SimplyMediaPlayer::Stop()
{
    if(!BASS_ChannelStop(MainChannel))
    {
        return 0;
    }

    return 1;
}

int SimplyMediaPlayer::Pause()
{
    if(!BASS_ChannelPause(MainChannel))
    {
        return 0;
    }

    return 1;
}

int SimplyMediaPlayer::Forward(QSlider *slider)
{
    if(BASS_ChannelIsActive(MainChannel) == BASS_ACTIVE_PAUSED || BASS_ChannelIsActive(MainChannel) == BASS_ACTIVE_STOPPED)
    {
         BASS_ChannelPause(MainChannel);
    }

    BASS_ChannelSetPosition(MainChannel, BASS_ChannelSeconds2Bytes(MainChannel,slider->maximum()-0.5f), BASS_POS_BYTE);
    BASS_ChannelPlay(MainChannel, FALSE);

    return 1;
}

int SimplyMediaPlayer::Backward(QSlider *slider)
{
  /*  BASS_ChannelPause(MainChannel);

    BASS_ChannelSetPosition(MainChannel, BASS_ChannelSeconds2Bytes(MainChannel,slider.maximum()-0.5f), BASS_POS_BYTE);
    BASS_ChannelPlay(MainChannel, FALSE);*/
}


int SimplyMediaPlayer::GetPlayerTime()
{
    return BASS_ChannelBytes2Seconds(MainChannel, BASS_ChannelGetLength(MainChannel,BASS_POS_BYTE));
}

int SimplyMediaPlayer::GetPlayerCurrentTime()
{
    return BASS_ChannelBytes2Seconds(MainChannel, BASS_ChannelGetPosition(MainChannel,BASS_POS_BYTE));
}

float SimplyMediaPlayer::GetVolume()
{
  return LastVolume;
}

PTime SimplyMediaPlayer::GetPTime()
{
    PTime temp;
    temp.minutes = (BASS_ChannelBytes2Seconds(MainChannel, BASS_ChannelGetLength(MainChannel,BASS_POS_BYTE))/60);
    temp.seconds = (temp.minutes-(int)temp.minutes)*60;

    return temp;
}

PTime SimplyMediaPlayer::GetPTime(QString file)
{

    PTime temp;

    QByteArray bytes = file.toLatin1();
    filename = bytes.data();

    QFileInfo pathInfo(file);

    LastPath = file;
    LastFileName = pathInfo.fileName();

    BASS_ChannelStop(TempChannel);
    BASS_MusicFree(TempChannel);
    BASS_StreamFree(TempChannel);

    if(!(TempChannel = BASS_StreamCreateFile(FALSE,filename,0,0,BASS_SAMPLE_LOOP))
    && !(TempChannel = BASS_MusicLoad(FALSE,filename,0,0,BASS_MUSIC_RAMP|BASS_SAMPLE_LOOP,0)))
    {

    }

    temp.minutes = (BASS_ChannelBytes2Seconds(TempChannel, BASS_ChannelGetLength(TempChannel,BASS_POS_BYTE))/60);
    temp.seconds = (temp.minutes-(int)temp.minutes)*60;

    return temp;
}

PTime SimplyMediaPlayer::GetCurrentPTime()
{
    PTime temp;
    temp.minutes = (BASS_ChannelBytes2Seconds(MainChannel, BASS_ChannelGetPosition(MainChannel,BASS_POS_BYTE))/60);
    temp.seconds = (temp.minutes-(int)temp.minutes)*60;

    return temp;
}

void SimplyMediaPlayer::ChangePosition(int position)
{
    if(BASS_ChannelIsActive(MainChannel) == BASS_ACTIVE_PAUSED)
    {
      BASS_ChannelPause(MainChannel);
      BASS_ChannelSetPosition(MainChannel, BASS_ChannelSeconds2Bytes(MainChannel,position), BASS_POS_BYTE);
    }
    else
    {
      BASS_ChannelPause(MainChannel);
      BASS_ChannelSetPosition(MainChannel, BASS_ChannelSeconds2Bytes(MainChannel,position), BASS_POS_BYTE);
      BASS_ChannelPlay(MainChannel, FALSE);
    }
}

int SimplyMediaPlayer::GetState()
{
    if(BASS_ChannelIsActive(MainChannel) == BASS_ACTIVE_PLAYING)
            return 1;

    if(BASS_ChannelIsActive(MainChannel) == BASS_ACTIVE_PAUSED)
            return 2;

    if(BASS_ChannelIsActive(MainChannel) == BASS_ACTIVE_STOPPED)
            return 3;
}

QPixmap SimplyMediaPlayer::DrawSpectrum(QWidget *ui, int x, int y, int w, int h, bool returnOnly)
{

}

QPixmap SimplyMediaPlayer::GetLastSpectrumRender(QWidget *ui)
{

}

int SimplyMediaPlayer::SetVolume(int vol)
{

    BASS_ChannelSetAttribute(MainChannel, BASS_ATTRIB_VOL, (float)(vol)/100.0f);
    LastVolume = vol;
    return LastVolume;
}

int SimplyMediaPlayer::SetBalance(int bal)
{

        BASS_ChannelSetAttribute(MainChannel, BASS_ATTRIB_PAN, (float)(bal*2)/100.0f);
        LastBalance = bal*2;
        return LastBalance;
}

QString SimplyMediaPlayer::GetLastPath()
{
    return LastPath;
}

QString SimplyMediaPlayer::GetLastFileName()
{
    return LastFileName;
}

void SimplyMediaPlayer::SetLastFileName(QString name)
{
    LastFileName = name;
}

void SimplyMediaPlayer::SetLastPath(QString path)
{
    LastPath = path;
}

DWORD SimplyMediaPlayer::GetChannel()
{
    return MainChannel;
}

int SimplyMediaPlayer::GetBufferDataSize()
{
   // QWORD len=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_END); // file/buffer length
    QWORD buf=BASS_StreamGetFilePosition(MainChannel, BASS_FILEPOS_BUFFER); // buffer level
  //  float progress=buf*100.0/len; // percentage of buffer filled

   return buf;
}

int SimplyMediaPlayer::GetDataSize()
{
    QWORD len=BASS_StreamGetFilePosition(MainChannel, BASS_FILEPOS_END); // file/buffer length
   // QWORD buf=BASS_StreamGetFilePosition(MainChannel, BASS_FILEPOS_BUFFER); // buffer level
  //  float progress=buf*100.0/len; // percentage of buffer filled

   return len;
}



