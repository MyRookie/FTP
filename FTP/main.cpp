//
//  main.cpp
//  FTP
//
//  Created by rookie on 20/03/15.
//  Copyright (c) 2015 rookie. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>


void login( )
{
    int  control_sock;
    struct hostent *hp;
    struct sockaddr_in server;
    char read_buf[1024] = {0};
    int  read_len = 512;
    
    memset(&server, 0, sizeof(struct sockaddr_in));
    
    /* init socket */
    control_sock = socket(AF_INET, SOCK_STREAM, 0);
    hp = gethostbyname("p13-preview.freehostingeu.com");
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
    //server.sin_addr.s_addr = inet_addr("10.10.10.100");
    server.sin_family = AF_INET;
    //server.sin_port = htons(port);
    server.sin_port = htons(21);
    
    /* connect server */
    connect(control_sock,(struct sockaddr *)&server, sizeof(server));
    /* get some welcome informations from server */
    read(control_sock, read_buf, read_len);
    printf("read_buf1 : %s\n",read_buf);
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    char send_buf[100] = {0};
    char username[30] = "1818934_rookie";
    char password[30] = "allen00mo";
    /* command ”USER username\r\n” */
    sprintf(send_buf,"USER %s\r\n",username);
    write(control_sock, send_buf, strlen(send_buf));
    /* 331 User name okay, need password.” */
    read(control_sock, read_buf, read_len);
    
    /* command ”PASS password\r\n” */
    sprintf(send_buf,"PASS %s\r\n",password);
    /*  */
    write(control_sock, send_buf, strlen(send_buf));
    /* ”230 User logged in, proceed.” */
    memset(read_buf,0,1024);
    read(control_sock, read_buf, read_len);
    printf("read_buf2 : %s\n",read_buf);
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    /* command ”PASV\r\n” */
    sprintf(send_buf,"PASV\r\n");
    /* start passive model */
    write(control_sock, send_buf, strlen(send_buf));
    /*”227 Entering passive mode (<h1,h2,h3,h4,p1,p2>)” */
    memset(read_buf,0,1024);
    read(control_sock, read_buf, read_len);
    printf("read_buf3 : %s\n",read_buf);
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    
    char * buf_ptr = NULL;
    int port_num = 0;
    buf_ptr = strrchr(read_buf, ')');
    if (buf_ptr)
        *buf_ptr = '\0';
    
    buf_ptr = strrchr(read_buf, ',');
    *buf_ptr = '\0';
    port_num = atoi(buf_ptr + 1);
    
    buf_ptr = strrchr(read_buf, ',');
    *buf_ptr = '\0';
    port_num += atoi(buf_ptr + 1) * 256;
    printf("The data socket port is %d.\n", port_num);
    struct sockaddr_in data_server;
    data_server.sin_family = AF_INET;
    data_server.sin_addr.s_addr = inet_addr("p13-preview.freehostingeu.com");
    data_server.sin_port = htons(port_num);
    
    int data_sock;
    data_sock = socket(AF_INET, SOCK_STREAM, 0);
    connect(data_sock,(struct sockaddr *)&data_server, sizeof(data_server));
    /* command ”CWD dirname\r\n” */
    sprintf(send_buf,"CWD %s\r\n","rookiealgo.com");
    /* change dir */
    write(control_sock, send_buf, strlen(send_buf));
    /*250 Command okay.” */
    memset(read_buf,0,1024);
    read(control_sock, read_buf, read_len);
    printf("read_buf4 : %s\n",read_buf);
    
    /* command ”SIZE filename\r\n” */
    sprintf(send_buf,"SIZE %s\r\n","index.html");
    /* get the file size */
    write(control_sock, send_buf, strlen(send_buf));
    /* ”213 <size>” */
    memset(read_buf,0,1024);
    read(control_sock, read_buf, read_len);
    printf("read_buf5 : %s\n",read_buf);
    
    /* command ”RETR filename\r\n” */
    sprintf(send_buf,"RETR %s\r\n","index.html");
    /* download file */
    write(control_sock, send_buf, strlen(send_buf));
    /* 150 Opening data connection.” */
    memset(read_buf,0,1024);
    read(control_sock, read_buf, read_len);
    printf("read_buf6 : %s\n",read_buf);
#if 1
    /* create file */
    int file_handle;
    int rc;
    int ret_read = 0;
    char buf[512]={0};
    file_handle = open("tessssssssss", O_WRONLY|O_CREAT|O_TRUNC, S_IREAD|S_IWRITE);
    
    while(1){
        ret_read = read(data_sock, buf, read_len);
        printf("read_buf7 : %s\n",buf);
        
        write(file_handle, buf, ret_read);
        
        if(ret_read < read_len)
        {
            break;
        }
    }
    rc = close(file_handle);
#endif
    
}




int main(void)
{
    login();
    return 0;
}
