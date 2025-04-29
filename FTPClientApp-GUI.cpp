#include <ftp_library/FTPClient.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>

ftp_library::FTPClient FtpClient;
bool Connected = false;
std::string RemoteDir = "/";

Fl_Input *HostInput, *UserInput, *PassInput, *RemoteFileInput, *LocalFileInput, *RemoteDirInput;
Fl_Text_Display *OutputDisplay;
Fl_Text_Buffer *OutputBuffer;

/**
 * Appends a message to the output box and scrolls to the bottom.
 *
 * @param Msg The message string to log.
 */
void Log(const std::string &Msg)
{
    OutputBuffer->append((Msg + "\n").c_str());
    OutputDisplay->insert_position(OutputBuffer->length());
    OutputDisplay->show_insert_position();
}

/**
 * Attempts to connect to the FTP server using the host input.
 *
 * @param widget Unused.
 * @param data Unused.
 */
void ConnectCallback(Fl_Widget *, void *)
{
    std::string Host = HostInput->value();
    try
    {
        FtpClient.Connect(Host, 21);
        Connected = true;
        Log("Connected to " + Host);
    }
    catch (const std::exception &E)
    {
        Log(std::string("Connection failed: ") + E.what());
    }
}

/**
 * Authenticates to the FTP server with the provided username and password.
 *
 * @param widget Unused.
 * @param data Unused.
 */
void AuthenticateCallback(Fl_Widget *, void *)
{
    if (!Connected)
    {
        Log("Not connected.");
        return;
    }
    std::string User = UserInput->value();
    std::string Pass = PassInput->value();
    try
    {
        FtpClient.Authenticate(User, Pass);
        Log("Authenticated as " + User);
    }
    catch (const std::exception &E)
    {
        Log(std::string("Authentication failed: ") + E.what());
    }
}

/**
 * Lists the contents of the current remote directory and logs them.
 *
 * @param widget Unused.
 * @param data Unused.
 */
void ListCallback(Fl_Widget *, void *)
{
    if (!Connected)
    {
        Log("Not connected.");
        return;
    }
    try
    {
        std::vector<std::string> Files = FtpClient.ListDirectory(RemoteDir);
        Log("Directory listing:");
        for (const auto &File : Files)
        {
            Log("  " + File);
        }
    }
    catch (const std::exception &E)
    {
        Log(std::string("List failed: ") + E.what());
    }
}

/**
 * Downloads a remote file to a local path.
 *
 * @param widget Unused.
 * @param data Unused.
 */
void DownloadCallback(Fl_Widget *, void *)
{
    if (!Connected)
    {
        Log("Not connected.");
        return;
    }
    std::string RemoteFile = RemoteFileInput->value();
    std::string LocalFile = LocalFileInput->value();
    if (LocalFile.empty() || LocalFile == ".")
    {
        size_t Pos = RemoteFile.find_last_of('/');
        LocalFile = RemoteFile.substr(Pos + 1);
    }
    try
    {
        FtpClient.DownloadFile(RemoteFile, LocalFile);
        Log("Downloaded " + RemoteFile + " to " + LocalFile);
    }
    catch (const std::exception &E)
    {
        Log(std::string("Download failed: ") + E.what());
    }
}

/**
 * Uploads a local file to the specified remote path.
 *
 * @param widget Unused.
 * @param data Unused.
 */
void UploadCallback(Fl_Widget *, void *)
{
    if (!Connected)
    {
        Log("Not connected.");
        return;
    }
    std::string LocalFile = LocalFileInput->value();
    std::string RemoteFile = RemoteFileInput->value();
    try
    {
        FtpClient.UploadFile(LocalFile, RemoteFile);
        Log("Uploaded " + LocalFile + " to " + RemoteFile);
    }
    catch (const std::exception &E)
    {
        Log(std::string("Upload failed: ") + E.what());
    }
}

/**
 * Changes the current working remote directory.
 *
 * @param widget Unused.
 * @param data Unused.
 */
void ChangeDirectoryCallback(Fl_Widget *, void *)
{
    if (!Connected)
    {
        Log("Not connected.");
        return;
    }
    RemoteDir = RemoteDirInput->value();
    Log("Changed remote directory to: " + RemoteDir);
}

/**
 * Disconnects from the FTP server.
 *
 * @param widget Unused.
 * @param data Unused.
 */
void DisconnectCallback(Fl_Widget *, void *)
{
    if (!Connected)
    {
        Log("Not connected.");
        return;
    }
    try
    {
        FtpClient.Disconnect();
        Connected = false;
        Log("Disconnected.");
    }
    catch (const std::exception &E)
    {
        Log(std::string("Disconnect failed: ") + E.what());
    }
}

/**
 * Initializes and runs the FTP client GUI application.
 *
 * @return Exit status code.
 */
int main()
{
    Fl_Window *Window = new Fl_Window(600, 500, "FTP Client GUI");

    HostInput = new Fl_Input(100, 10, 200, 25, "Host:");
    UserInput = new Fl_Input(100, 40, 200, 25, "Username:");
    PassInput = new Fl_Input(100, 70, 200, 25, "Password:");
    PassInput->type(FL_SECRET_INPUT);

    Fl_Button *ConnectBtn = new Fl_Button(320, 10, 100, 25, "Connect");
    ConnectBtn->callback(ConnectCallback);

    Fl_Button *AuthBtn = new Fl_Button(320, 40, 100, 25, "Authenticate");
    AuthBtn->callback(AuthenticateCallback);

    Fl_Button *ListBtn = new Fl_Button(430, 10, 100, 25, "List Dir");
    ListBtn->callback(ListCallback);

    RemoteFileInput = new Fl_Input(100, 110, 200, 25, "Remote File:");
    LocalFileInput = new Fl_Input(100, 140, 200, 25, "Local File:");

    Fl_Button *DownloadBtn = new Fl_Button(320, 110, 100, 25, "Download");
    DownloadBtn->callback(DownloadCallback);

    Fl_Button *UploadBtn = new Fl_Button(320, 140, 100, 25, "Upload");
    UploadBtn->callback(UploadCallback);

    RemoteDirInput = new Fl_Input(100, 180, 200, 25, "Remote Dir:");
    Fl_Button *CdBtn = new Fl_Button(320, 180, 100, 25, "Change Dir");
    CdBtn->callback(ChangeDirectoryCallback);

    Fl_Button *DisconnectBtn = new Fl_Button(430, 40, 100, 25, "Disconnect");
    DisconnectBtn->callback(DisconnectCallback);

    OutputBuffer = new Fl_Text_Buffer();
    OutputDisplay = new Fl_Text_Display(20, 220, Window->w() - 40, Window->h() - 240, "Output:");
    OutputDisplay->buffer(OutputBuffer);
    OutputDisplay->box(FL_DOWN_BOX);
    OutputDisplay->textfont(FL_COURIER);
    OutputDisplay->textsize(12);
    OutputDisplay->scrollbar_align(FL_ALIGN_RIGHT);

    Window->resizable(*OutputDisplay);

    Window->end();
    Window->show();
    return Fl::run();
}
