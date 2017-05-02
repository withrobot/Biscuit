
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <syslog.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/stat.h>


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <ifaddrs.h>




#define TOOLBAR_ICON
#ifdef TOOLBAR_ICON
#include <gtk/gtk.h>
#include <sys/wait.h>
#define TOOLBAR_LABEL_DEFAULT "Mouse Server"
#define TOOLBAR_ICON_DEFAULT "/usr/share/mouseserver/icons/ms-default.svg"
#define TOOLBAR_ICON_CONNECTED "/usr/share/mouseserver/icons/ms-connected.svg"
#endif

#include "avahi.hpp"
#include "session.hpp"

#include "version.hpp.in"

#ifdef TOOLBAR_ICON
void* GTKStartup(void *);
GtkStatusIcon* tray = NULL;
#endif
char path[PATH_MAX];
int _argc;
char** _argv;

void *udpsender(void *) {
    int sockfd;
    struct sockaddr_in addr;
    char buffer[128];

    sockfd = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd < 0) {
        fprintf(stderr,"Socket Error:%sn",strerror(errno));
        exit(1);
    }
    
    const int on=1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    bzero(&addr,sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2008);
    addr.sin_addr.s_addr = inet_addr("255.255.255.255");
/*    if(inet_aton("255.255.255.255",&addr.sin_addr) < 0) {
        fprintf(stderr,"IP error:%sn",strerror(errno));
	return NULL;
    }
*/
    while(1) {
	bzero(buffer, 128);
	char shostname[64], shostlength[3];
	bzero(shostname, 64);
	bzero(shostlength, 3);
	
	if (gethostname(shostname, 64) < 0)
		strcpy(shostname, "ubuntu");

	//printf("get hostname : %s\n", shostname);

	int hlength = strlen(shostname);
	sprintf(shostlength, "%d", hlength);

	strcpy(buffer, "BC");	
	if (hlength < 10) {
		strcat(buffer, "  ");
	}else {
		strcat(buffer, " ");
	}
	
	strcat(buffer, shostlength); 	
	
	strcat(buffer, shostname);
	
	//printf("send buffer :%s\n", buffer);
	if (sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&addr,sizeof(addr)) <= 0){
		//printf("sendto error");	
	} 
	
	sleep(1);
    }
    
    close(sockfd);
    return NULL;
}

void *udpserver(void *) {
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); /* create a socket */

	/* init servaddr */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(1980);

	/* bind address and port to socket */
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
	perror("bind error");
	exit(1);
	}
	

	char mesg[256];
	
	socklen_t slen = sizeof(cliaddr);

	while(1) {

	int n = recvfrom(sockfd, mesg, 256, 0, (struct sockaddr *)&cliaddr, &slen);
	
	if (n > 0){
		mesg[n] = 0;
		process(mesg);
	}

	}
		


}

int main(int argc, char* argv[])
{
	signal(SIGPIPE, SIG_IGN);
	
	// store for later
	_argc = argc;
	_argv = argv;

	/* syslog */
	int logOpt = LOG_PID | LOG_CONS | LOG_PERROR;
	openlog("mouse server", logOpt, LOG_DAEMON); 
	
	
	
	short server_port = 1978;

	syslog(LOG_INFO, "started on port %d", server_port);
	daemon(1, 1);


	/* bind.. */
	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof serv_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(server_port);

	/* setup network.. */
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		syslog(LOG_ERR, "socket: %s", strerror(errno));
		exit(1);
	}

	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0)
	{
		syslog(LOG_ERR, "setsockopt: %s", strerror(errno));
		exit(1);
	}

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof serv_addr) < 0)
	{
		syslog(LOG_ERR, "bind: %s", strerror(errno));
		exit(1);
	}

	if (listen(sockfd, 1) < 0)
	{
		syslog(LOG_ERR, "listen: %s", strerror(errno));
		exit(1);
	}

#ifdef TOOLBAR_ICON
	pthread_t toolbarpid;
	if (pthread_create(&toolbarpid, 0x0, GTKStartup, (void*)0x0) == -1)
	{
		syslog(LOG_WARNING, "pthread_create failed: %s", strerror(errno));
	}
#endif

	//start udp 
	{
		pthread_t pid;
		if (pthread_create(&pid, 0x0, udpsender, NULL) == -1)
		{
			syslog(LOG_ERR, "pthread_create udpsender failed");
			
		}
	}

	
	{
		pthread_t pid;
		if (pthread_create(&pid, 0x0, udpserver, NULL) == -1)
		{
			syslog(LOG_ERR, "pthread_create udpserver failed");
			
		}
	}

	/* server loop.. */
	while(1)
	{
		struct sockaddr_in caddr;
		int clen = sizeof caddr;

		/* accept client.. */
		int client = accept(sockfd, (struct sockaddr *)&caddr, (socklen_t*)&clen);
		if (client < 0)
		{
			syslog(LOG_WARNING, "accept failed: %s", strerror(errno));
			continue;
		}

		/* start new session.. */
		SessionContext * clientContext = new SessionContext(
				client,
				inet_ntoa(caddr.sin_addr));

#ifdef TOOLBAR_ICON
		gtk_status_icon_set_tooltip(tray, std::string(clientContext->m_address + " connected").c_str());
		gtk_status_icon_set_from_file(tray, TOOLBAR_ICON_CONNECTED);
#endif

		pthread_t pid;
		if (pthread_create(&pid, 0x0, MobileMouseSession, (void*)clientContext) == -1)
		{
			syslog(LOG_WARNING, "pthread_create failed: %s", strerror(errno));
			delete clientContext;
			close(client);
		} else 
			pthread_join(pid, 0x0);

#ifdef TOOLBAR_ICON
		gtk_status_icon_set_tooltip(tray, TOOLBAR_LABEL_DEFAULT);
		gtk_status_icon_set_from_file(tray, TOOLBAR_ICON_DEFAULT);
#endif
	}

	return 0;
}

void GetIP(char *s)

{

  int socket_fd;
  //struct sockaddr_in *sin;
  struct ifreq *ifr;
  struct ifconf conf;
  char buff[BUFSIZ];
  int num;
  int i;
  socket_fd = socket(AF_INET,SOCK_DGRAM,0);
  conf.ifc_len = BUFSIZ;
  conf.ifc_buf = buff;
  ioctl(socket_fd,SIOCGIFCONF,&conf);
  num = conf.ifc_len / sizeof(struct ifreq);
  ifr = conf.ifc_req;
  //printf("num=%d\n",num);
  for(i=0;i<num;i++)
  {
    struct sockaddr_in *sin = (struct sockaddr_in *)(&ifr->ifr_addr);

    ioctl(socket_fd,SIOCGIFFLAGS,ifr);
    if(((ifr->ifr_flags & IFF_LOOPBACK) == 0) && (ifr->ifr_flags & IFF_UP))
    {
      //printf("%s(%s)\n",ifr->ifr_name,inet_ntoa(sin->sin_addr));
      strcpy(s, inet_ntoa(sin->sin_addr));
    }
    ifr++;
  }

}


#ifdef TOOLBAR_ICON
void GTKTrayAbout(GtkMenuItem* item __attribute__((unused)), gpointer uptr __attribute__((unused))) 
{
	const gchar* authors[] = { "Shimeng Wang <wangshimeng@gmail.com>", NULL }; 
	const gchar* license = "GNU GENERAL PUBLIC LICENSE\nVersion 2, Jan 2013\n\nhttp://www.gnu.org/licenses/gpl-2.0.txt";
	
	
	GtkWidget* about = gtk_about_dialog_new();
	gtk_about_dialog_set_name((GtkAboutDialog*)about, "Mouse Server for Linux");
	gtk_window_set_icon((GtkWindow*)about, gtk_widget_render_icon(about, GTK_STOCK_ABOUT, GTK_ICON_SIZE_MENU, NULL));
	gtk_about_dialog_set_copyright((GtkAboutDialog*)about, "Copyright (C) 2013 Necta, All Rights Reserved");
	gtk_about_dialog_set_website((GtkAboutDialog*)about, "http://wifimouse.necta.us/");
	gtk_about_dialog_set_comments((GtkAboutDialog*)about, "WiFi Mouse transform your phone into wireless mouse, keyboard and trackpad. WiFi Mouse supports speech-to-text as well as multi-finger trackpad gestures. And it enable you to control your PC,MAC or HTPC effortlessly through a local network connection.");
	gtk_about_dialog_set_authors((GtkAboutDialog*)about, authors);
	gtk_about_dialog_set_license((GtkAboutDialog*)about, license);

	gtk_dialog_run((GtkDialog*)about);
	gtk_about_dialog_set_version((GtkAboutDialog*)about, MMSERVER_VERSION_MAJOR"."MMSERVER_VERSION_MINOR"."MMSERVER_VERSION_PATCH);
	gtk_widget_destroy(about);
}

void destroy(GtkWidget *widget, gpointer *data) {
	GtkWidget * t = widget;
	if (t != NULL)
	gtk_widget_destroy(GTK_WIDGET(data));
	
}

void make_dialog()
{
	GtkWidget *dialog;
	GtkWidget *label;
	GtkWidget *button;
	GtkWidget *vbox;
	GtkWidget *hbox;

	
	char strip[32], allip[64];
	memset(strip, 0, sizeof(strip));
	memset(allip, 0, sizeof(allip));

	GetIP(strip);
	
	strcpy(allip, "   IP address:");
	strcat(allip, strip);
	strcat(allip, "   ");

	dialog = gtk_dialog_new();

	vbox = GTK_DIALOG(dialog)->vbox;
	label = gtk_label_new(allip);
	gtk_box_pack_start(GTK_BOX(vbox),label,TRUE,TRUE,30);
	
	
	hbox = GTK_DIALOG(dialog)->action_area;
	button = gtk_button_new_with_label("OK");
	gtk_box_pack_start(GTK_BOX(hbox), button, TRUE, TRUE, 0);
	gtk_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(destroy), dialog);
 
	gtk_window_set_title(GTK_WINDOW(dialog), "Show IP address");
	gtk_widget_show_all(dialog);
}



void GTKPreferences(GtkMenuItem* item __attribute__((unused)), gpointer uptr __attribute__((unused))) 
{
	make_dialog();
}

void GTKTrayQuit(GtkMenuItem* item __attribute__((unused)), gpointer uptr __attribute__((unused))) 
{
	syslog(LOG_INFO, "quit");
	gtk_main_quit();
}

void GTKTrayMenu(GtkStatusIcon* tray, guint button, guint32 time, gpointer uptr)
{
	gtk_menu_popup(GTK_MENU(uptr), NULL, NULL, gtk_status_icon_position_menu, tray, button, time);
}

void* GTKStartup(void *)
{
	gtk_init(0, 0x0);
	tray = gtk_status_icon_new();
	
	GtkWidget *menu = gtk_menu_new(),
		  *menuPreferences = gtk_menu_item_new_with_label("Show IP address"),
		  *menuAbout = gtk_menu_item_new_with_label("About"),
		  *menuQuit = gtk_menu_item_new_with_label("Quit");
	g_signal_connect(G_OBJECT(menuPreferences), "activate", G_CALLBACK(GTKPreferences), NULL);
	g_signal_connect(G_OBJECT(menuAbout), "activate", G_CALLBACK(GTKTrayAbout), NULL);
	g_signal_connect(G_OBJECT(menuQuit), "activate", G_CALLBACK(GTKTrayQuit), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuPreferences);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuAbout);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), gtk_separator_menu_item_new());
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuQuit);
	gtk_widget_show_all(menu);

	g_signal_connect(G_OBJECT(tray), "popup-menu", G_CALLBACK(GTKTrayMenu), menu);
	gtk_status_icon_set_tooltip(tray, TOOLBAR_LABEL_DEFAULT);
	gtk_status_icon_set_from_icon_name(tray, TOOLBAR_ICON_DEFAULT);
	gtk_status_icon_set_from_file(tray, TOOLBAR_ICON_DEFAULT);
	gtk_status_icon_set_visible(tray, TRUE);

	gtk_main();
	exit(0);
}
#endif
