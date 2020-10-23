/*
 * Copyright (c) 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2005 Mellanox Technologies Ltd.  All rights reserved.
 * Copyright (c) 2009 HNR Consulting.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * $Id$
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <pthread.h> 

#include "perftest_parameters.h"
#include "perftest_resources.h"
#include "perftest_communication.h"

// Let us create a global variable to change it in threads 
int g = 0; 
  
// The function to be executed by all threads 
/*void *myThreadFun(void *vargp) 
{ 
    // Store the value argument passed to this thread 
    int *myid = (int *)vargp; 
  
    // Let us create a static variable to observe its changes 
    static int s = 0; 
  
    // Change static and global variables 
    ++s; ++g; 
  
    // Print the argument, static and global variables 
    printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g); 
} */
/******************************************************************************
 ******************************************************************************/
 
 struct bw_args {
    struct pingpong_context* ctx;
    struct perftest_parameters* user_param;
};

 
int main(int argc, char *argv[])
{
/*
	int q; 
    pthread_t tid; 
  
    // Let us create three threads 
    for (q = 0; q < 3; q++) 
        pthread_create(&tid, NULL, myThreadFun, (void *)&tid); 
  
    pthread_exit(NULL); 
    return 0; 
*/
	
	
	
	
	
	
	
  printf("##Begin Main\n");
	int				ret_parser, i = 0, rc;
  //int				ret_parser2;
//	struct ibv_device		*ib_dev = NULL;
  struct ibv_device		*ib_dev_dual1 = NULL;
  struct ibv_device		*ib_dev_dual2 = NULL;
  
  //struct pingpong_context		ctx;
	struct pingpong_context		ctx1;
  struct pingpong_context		ctx2;
 
	//struct pingpong_dest		*my_dest,*rem_dest;
  struct pingpong_dest		*my_dest1,*rem_dest1;
  struct pingpong_dest		*my_dest2,*rem_dest2;
   
	//struct perftest_parameters	user_param;
  struct perftest_parameters	user_param1;
  struct perftest_parameters	user_param2;
 
	//struct perftest_comm		user_comm;
  struct perftest_comm		user_comm1;
  struct perftest_comm		user_comm2;
 	//struct bw_report_data		my_bw_rep, rem_bw_rep;
  struct bw_report_data		my_bw_rep1, rem_bw_rep1;
  struct bw_report_data		my_bw_rep2, rem_bw_rep2;

	pthread_t dev1_thread, dev2_thread;
    int  dev1_thread_err, dev2_thread_err; 


	/* init default values to user's parameters */
	//memset(&user_param,0,sizeof(struct perftest_parameters));
 	memset(&user_param1,0,sizeof(struct perftest_parameters));
 	memset(&user_param2,0,sizeof(struct perftest_parameters));
  
  //memset(&user_comm,0,sizeof(struct perftest_comm));
	memset(&user_comm1,0,sizeof(struct perftest_comm));
  memset(&user_comm2,0,sizeof(struct perftest_comm));
 
  //memset(&ctx,0,sizeof(struct pingpong_context));
	memset(&ctx1,0,sizeof(struct pingpong_context));
  memset(&ctx2,0,sizeof(struct pingpong_context));

	user_param1.verb    = WRITE;
	user_param1.tst     = BW;
	
	user_param2.verb    = WRITE;
	user_param2.tst     = BW;
	
	
	strncpy(user_param1.version, VERSION, sizeof(user_param1.version));
	strncpy(user_param2.version, VERSION, sizeof(user_param2.version));
	
	printf("Version\n");
	/* Configure the parameters values according to user arguments or default values. */
	ret_parser = parser(&user_param1,argv,argc);
	//ret_parser = parser(&user_param2,argv,argc);
	if (ret_parser) {
		if (ret_parser != VERSION_EXIT && ret_parser != HELP_EXIT)
			fprintf(stderr," Parser function exited with Error\n");
		return FAILURE;
	}
	user_param2=user_param1;
	if((user_param1.connection_type == DC || user_param1.use_xrc) && user_param1.duplex) {
		user_param1.num_of_qps *= 2;
	}
	if((user_param2.connection_type == DC || user_param2.use_xrc) && user_param2.duplex) {
		user_param2.num_of_qps *= 2;
	}
  //create the user_params for each device
  //user_param1=user_param;
  //user_param2=user_param;
  
  printf("Dev         : %s\n",user_param2.ib_devname);
  printf("Dev1         : %s\n", user_param2.ib_devname1);
  printf("Dev2         : %s\n",user_param2.ib_devname2);
  
  //user_param1 = user_param;
  user_param1.ib_devname=user_param1.ib_devname1;
  user_param1.port=user_param1.port;
  
  printf("Dev         : %s\n",user_param2.ib_devname);
  printf("Dev1         : %s\n", user_param2.ib_devname1);
  printf("Dev2         : %s\n",user_param2.ib_devname2);
  
  //user_param2 = user_param;
  user_param2.ib_devname=user_param2.ib_devname2;
  user_param2.port=user_param2.port+1;
  
  

/* Finding the IB device selected (or default if none is selected). */
  //user_param1.ib_devname = "mlx5_0";
  ib_dev_dual1 = ctx_find_dev(user_param1.ib_devname);
	printf("Device1         : %s\n", user_param1.ib_devname);
  if (!ib_dev_dual1) {
		fprintf(stderr," Unable to find the Infiniband/RoCE device1\n");
		return FAILURE;
	}
  
/* Finding the IB device selected (or default if none is selected). */
  //user_param2.ib_devname = "mlx5_2";
  

  ib_dev_dual2 = ctx_find_dev(user_param2.ib_devname);
  printf("Device2         : %s\n", user_param2.ib_devname);
	if (!ib_dev_dual2) {
		fprintf(stderr," Unable to find the Infiniband/RoCE device2\n");
		return FAILURE;
	}
  


   printf("Port1         : %d\n", user_param1.port);
   printf("Port2         : %d\n", user_param2.port);
   
   printf("IP         : %s\n",   user_param1.servername);  
   printf("IP2         : %s\n",   user_param2.servername);
   
     
  //user_param = user_param2;
  
	/* Getting the relevant context from the device */

	ctx1.context = ibv_open_device(ib_dev_dual1);
	if (!ctx1.context) {
		fprintf(stderr, " Couldn't get context for the device1\n");
		return FAILURE;
	}

	/* Getting the relevant context from the device */
	ctx2.context = ibv_open_device(ib_dev_dual2);
	if (!ctx2.context) {
		fprintf(stderr, " Couldn't get context for the device2\n");
		return FAILURE;
  }

	/* See if MTU and link type are valid and supported. */
	if (check_link(ctx1.context,&user_param1)) {
		fprintf(stderr, " Couldn't get context for the device MTU1\n");
		return FAILURE;
	}

	/* See if MTU and link type are valid and supported. */
	if (check_link(ctx2.context,&user_param2)) {
		fprintf(stderr, " Couldn't get context for the device MTU2\n");
		return FAILURE;
	}
 
 	/* copy the relevant user parameters to the comm struct + creating rdma_cm resources. */
	if (create_comm_struct(&user_comm1,&user_param1)) {
		fprintf(stderr," Unable to create RDMA_CM resources\n");
		return FAILURE;
	}
 
 	/* copy the relevant user parameters to the comm struct + creating rdma_cm resources. */
	if (create_comm_struct(&user_comm2,&user_param2)) {
		fprintf(stderr," Unable to create RDMA_CM resources\n");
		return FAILURE;
	}
 
   printf("Devicecreatecomm         : %s\n", user_param1.ib_devname);
   printf("Port         : %d\n", user_param1.port);
   printf("Device2         : %s\n", user_param2.ib_devname);
   printf("Port2         : %d\n", user_param2.port);
   printf("IP         : %s\n",   user_param1.servername);  
   printf("IP2         : %s\n",   user_param2.servername);

	if (user_param1.output == FULL_VERBOSITY && user_param1.machine == SERVER) {
		printf("\n************************************\n");
		printf("* Waiting for client to connect... *\n");
		printf("************************************\n");
	}

	/* Initialize the connection and print the local data. */
	if (establish_connection(&user_comm1)) {
		fprintf(stderr," Unable to init the socket connection1\n");
		return FAILURE;
	}
	sleep(3);
	printf("Established1\n");

	
  if (establish_connection(&user_comm2)) {
		fprintf(stderr," Unable to init the socket connection2\n");
		return FAILURE;
	}
	sleep(3);

	printf("Established2\n");
    
   //printf("Device         : %s\n", user_param.ib_devname);
   //printf("Port1         : %d\n", user_param1.port);
   //printf("Device         : %s\n", user_param2.ib_devname);
   //printf("Port2         : %d\n", user_param2.port);
   //printf("IP         : %s\n",   user_param1.servername);  
   //printf("IP2         : %s\n",   user_param2.servername);
   
	exchange_versions(&user_comm1, &user_param1);
	check_sys_data(&user_comm1, &user_param1);
	printf("CheckVersions\n");
/*
	exchange_versions(&user_comm1, &user_param1);
	check_sys_data(&user_comm1, &user_param1);
 	printf("CheckVersions1\n");
*/  
 	exchange_versions(&user_comm2, &user_param2);
	check_sys_data(&user_comm2, &user_param2);
	printf("CheckVersions2\n");
 
   //  printf("Device         : %s\n", user_param.ib_devname);
   printf("Port1         : %d\n", user_param1.port);
//   printf("Device         : %s\n", user_param1.ib_devname);
//   printf("Port         : %d\n", user_param1.port);
   //printf("Device         : %s\n", user_param2.ib_devname);
   printf("Port2         : %d\n", user_param2.port);
   
   printf("IP         : %s\n",   user_param1.servername);  
   printf("IP2         : %s\n",   user_param2.servername);
 

 
 	/* See if MTU and link type are valid and supported. */

	if (check_mtu(ctx1.context,&user_param1, &user_comm1)) {
		fprintf(stderr, " Couldn't get context for the device\n");
		return FAILURE;
	}

 	/* See if MTU and link type are valid and supported. */
	if (check_mtu(ctx2.context,&user_param2, &user_comm2)) {
		fprintf(stderr, " Couldn't get context for the device\n");
		return FAILURE;
	}
 
    //printf("Device         : %s\n", user_param.ib_devname);
   printf("Port1         : %d\n", user_param1.port);
//   printf("Device         : %s\n", user_param1.ib_devname);
//   printf("Port         : %d\n", user_param1.port);
   //printf("Device         : %s\n", user_param2.ib_devname);
   printf("Port2         : %d\n", user_param2.port);
   
   printf("IP         : %s\n",   user_param1.servername);  
   printf("IP2         : %s\n",   user_param2.servername);



	ALLOCATE(my_dest1 , struct pingpong_dest , user_param1.num_of_qps);
	memset(my_dest1, 0, sizeof(struct pingpong_dest)*user_param1.num_of_qps);
	ALLOCATE(rem_dest1 , struct pingpong_dest , user_param1.num_of_qps);
	memset(rem_dest1, 0, sizeof(struct pingpong_dest)*user_param1.num_of_qps);

 	ALLOCATE(my_dest2 , struct pingpong_dest , user_param2.num_of_qps);
	memset(my_dest2, 0, sizeof(struct pingpong_dest)*user_param2.num_of_qps);
	ALLOCATE(rem_dest2 , struct pingpong_dest , user_param2.num_of_qps);
	memset(rem_dest2, 0, sizeof(struct pingpong_dest)*user_param2.num_of_qps);

	/* Allocating arrays needed for the test. */
	alloc_ctx(&ctx1,&user_param1);
//  alloc_ctx(&ctx1,&user_param1);
  alloc_ctx(&ctx2,&user_param2);

	/* Create RDMA CM resources and connect through CM. */
	if (user_param1.work_rdma_cm == ON) {
    printf ("Create RDMA CM resources and connect through CM.");
		rc = create_rdma_cm_connection(&ctx1, &user_param1, &user_comm1,
			my_dest1, rem_dest1);
		if (rc) {
			fprintf(stderr,
				"Failed to create RDMA CM connection with resources.\n");
			return FAILURE;
		}
	} else {
		/* create all the basic IB resources (data buffer, PD, MR, CQ and events channel) */
		printf ("create all the basic IB resources (data buffer, PD, MR, CQ and events channel)");
    if (ctx_init(&ctx1, &user_param1)) {
			fprintf(stderr, " Couldn't create IB resources\n");
			return FAILURE;
		}
    if (ctx_init(&ctx2, &user_param2)) {
			fprintf(stderr, " Couldn't create IB resources\n");
			return FAILURE;
		}
	}

	/* Set up the Connection. */
	if (set_up_connection(&ctx1,&user_param1,my_dest1)) {
		fprintf(stderr," Unable to set up socket connection\n");
		return FAILURE;
	}
  
 	if (set_up_connection(&ctx2,&user_param2,my_dest2)) {
		fprintf(stderr," Unable to set up socket connection\n");
		return FAILURE;
	}

	/* Print basic test information. */
 /*
 ------------------------------------------
                    RDMA_Write BW Test
 Dual-port       : OFF          Device         : mlx5_1
 Number of qps   : 1            Transport type : IB
 Connection type : RC           Using SRQ      : OFF
 CQ Moderation   : 100
 Mtu             : 4096[B]
 Link type       : IB
 Max inline data : 0[B]
 rdma_cm QPs     : OFF
 Data ex. method : Ethernet
---------------------------------------------------------------------------------------
*/
	ctx_print_test_info(&user_param1);
 	ctx_print_test_info(&user_param2);

// local address: LID 0x07 QPN 0x0888 PSN 0x483bca RKey 0x087b55 VAddr 0x007ff5fe76e000

	/* Print this machine QP information */
	for (i=0; i < user_param1.num_of_qps; i++)
		ctx_print_pingpong_data(&my_dest1[i],&user_comm1);

//	for (i=0; i < user_param1.num_of_qps; i++)
//		ctx_print_pingpong_data(&my_dest1[i],&user_comm1);

	for (i=0; i < user_param2.num_of_qps; i++)
		ctx_print_pingpong_data(&my_dest2[i],&user_comm2);

// remote address: LID 0x05 QPN 0x08a4 PSN 0xddce55 RKey 0x0a1f7b VAddr 0x007fee79810000
	user_comm1.rdma_params->side = REMOTE;
	for (i=0; i < user_param1.num_of_qps; i++) {

		if (ctx_hand_shake(&user_comm1,&my_dest1[i],&rem_dest1[i])) {
			fprintf(stderr," Failed to exchange data between server and clients\n");
			return FAILURE;
		}

		ctx_print_pingpong_data(&rem_dest1[i],&user_comm1);
	}
 
 	user_comm2.rdma_params->side = REMOTE;
	for (i=0; i < user_param2.num_of_qps; i++) {

		if (ctx_hand_shake(&user_comm2,&my_dest2[i],&rem_dest2[i])) {
			fprintf(stderr," Failed to exchange data between server and clients\n");
			return FAILURE;
		}

		ctx_print_pingpong_data(&rem_dest2[i],&user_comm2);
	}

	if (user_param1.work_rdma_cm == OFF) {
		printf("user_param1.work_rdma_cm == OFF, ctx_check_gid_compatibility\n");
    if (ctx_check_gid_compatibility(&my_dest1[0], &rem_dest1[0])) {
			fprintf(stderr,"\n Found Incompatibility issue with GID types.\n");
			fprintf(stderr," Please Try to use a different IP version.\n\n");
			return FAILURE;
		}
	}

	if (user_param1.work_rdma_cm == OFF) {
    printf("user_param1.work_rdma_cm == OFF,ctx_connect\n");
		if (ctx_connect(&ctx1,rem_dest1,&user_param1,my_dest1)) {
			fprintf(stderr," Unable to Connect the HCA's through the link\n");
			return FAILURE;
		}
	}

	/* An additional handshake is required after moving qp to RTR. */
	if (ctx_hand_shake(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
		fprintf(stderr," Failed to exchange data between server and clients1\n");
		return FAILURE;
	}


	if (user_param2.work_rdma_cm == OFF) {
		printf("user_param2.work_rdma_cm == OFF, ctx_check_gid_compatibility\n");
    if (ctx_check_gid_compatibility(&my_dest2[0], &rem_dest2[0])) {
			fprintf(stderr,"\n Found Incompatibility issue with GID types.\n");
			fprintf(stderr," Please Try to use a different IP version.\n\n");
			return FAILURE;
		}
	}
	if (user_param2.work_rdma_cm == OFF) {
    printf("user_param2.work_rdma_cm == OFF,ctx_connect\n");
		if (ctx_connect(&ctx2,rem_dest2,&user_param2,my_dest2)) {
			fprintf(stderr," Unable to Connect the HCA's through the link\n");
			return FAILURE;
		}
	}
 

	/* An additional handshake is required after moving qp to RTR. */
	if (ctx_hand_shake(&user_comm2,&my_dest2[0],&rem_dest2[0])) {
		fprintf(stderr," Failed to exchange data between server and clients\n");
		return FAILURE;
	}


	if (user_param1.output == FULL_VERBOSITY) {
		if (user_param1.report_per_port) {
      //printf("user_param.report_per_port");
			printf(RESULT_LINE_PER_PORT);
			printf((user_param1.report_fmt == MBS ? RESULT_FMT_PER_PORT : RESULT_FMT_G_PER_PORT));
		}
		else {
			//---------------------------------------------------------------------------------------
      //#bytes     #iterations    BW peak[MB/sec]    BW average[MB/sec]   MsgRate[Mpps]u
      printf(RESULT_LINE);
			printf((user_param1.report_fmt == MBS ? RESULT_FMT : RESULT_FMT_G));
      
		}
  //Conflicting CPU frequency values detected: 2014.984000 != 2637.316000. CPU Frequency is not max.
		printf((user_param1.cpu_util_data.enable ? RESULT_EXT_CPU_UTIL : RESULT_EXT));
	}

	/* For half duplex tests, server just waits for client to exit */

	if (user_param1.machine == SERVER && !user_param1.duplex) {
    printf("half duplex\n");
    
		if (ctx_hand_shake(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
			fprintf(stderr," Failed to exchange data between server and clients1\n");
			return FAILURE;
		}
    printf("ctx_hand_shake done\n");
		
    xchg_bw_reports(&user_comm1, &my_bw_rep1,&rem_bw_rep1,atof(user_param1.rem_version));
    printf("xchg_bw_reports\n");
		print_full_bw_report(&user_param1, &rem_bw_rep1, NULL);


//	}
 
//	if (user_param2.machine == SERVER && !user_param2.duplex) {
    printf("half duplex2\n");
    
		if (ctx_hand_shake(&user_comm2,&my_dest2[0],&rem_dest2[0])) {
			fprintf(stderr," Failed to exchange data between server and clients2\n");
			return FAILURE;
		}
    printf("ctx_hand_shake done\n");
		
    xchg_bw_reports(&user_comm2, &my_bw_rep2,&rem_bw_rep2,atof(user_param2.rem_version));
    printf("xchg_bw_reports\n");
		print_full_bw_report(&user_param2, &rem_bw_rep2, NULL);


		if (ctx_close_connection(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
			fprintf(stderr,"Failed to close connection between server and client\n");
			return FAILURE;
		}

		if (user_param1.output == FULL_VERBOSITY) {
			if (user_param1.report_per_port)
				printf(RESULT_LINE_PER_PORT);
			else
				printf(RESULT_LINE);
		}

		if (user_param1.work_rdma_cm == ON) {
			if (destroy_ctx(&ctx1,&user_param1)) {
				fprintf(stderr, "Failed to destroy resources\n");
				return FAILURE;
			}

			user_comm1.rdma_params->work_rdma_cm = OFF;
			//return destroy_ctx(user_comm.rdma_ctx,user_comm.rdma_params);
      //return destroy_ctx(user_comm.rdma_ctx,user_comm.rdma_params);
      destroy_ctx(user_comm1.rdma_ctx,user_comm1.rdma_params);
		}

		//return destroy_ctx(&ctx,&user_param);
    destroy_ctx(&ctx1,&user_param1);

		if (ctx_close_connection(&user_comm2,&my_dest2[0],&rem_dest2[0])) {
			fprintf(stderr,"Failed to close connection between server and client\n");
			return FAILURE;
		}

		if (user_param2.output == FULL_VERBOSITY) {
			if (user_param2.report_per_port)
				printf(RESULT_LINE_PER_PORT);
			else
				printf(RESULT_LINE);
		}

		if (user_param2.work_rdma_cm == ON) {
			if (destroy_ctx(&ctx2,&user_param2)) {
				fprintf(stderr, "Failed to destroy resources\n");
				return FAILURE;
			}

			user_comm2.rdma_params->work_rdma_cm = OFF;
			//return destroy_ctx(user_comm.rdma_ctx,user_comm.rdma_params);
      return destroy_ctx(user_comm2.rdma_ctx,user_comm2.rdma_params);
		}
    
		return destroy_ctx(&ctx2,&user_param2);

	}

	if (user_param1.test_method == RUN_ALL) {
    printf("Run All");
		for (i = 1; i < 24 ; ++i) {

			user_param1.size = (uint64_t)1 << i;
			ctx_set_send_wqes(&ctx1,&user_param1,rem_dest1);
			ctx_set_send_wqes(&ctx2,&user_param2,rem_dest2);

			if (user_param1.perform_warm_up) {
				if(perform_warm_up(&ctx1, &user_param1)) {
					fprintf(stderr, "Problems with warm up\n");
					return FAILURE;
				}
			}

			if(user_param1.duplex) {
				if (ctx_hand_shake(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
					fprintf(stderr,"Failed to sync between server and client between different msg sizes\n");
					return FAILURE;
				}
			}

			if(run_iter_bw(&ctx1,&user_param1)) {
				fprintf(stderr," Failed to complete run_iter_bw function successfully\n");
				return FAILURE;
			}

			if (user_param1.duplex && (atof(user_param1.version) >= 4.6)) {
				if (ctx_hand_shake(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
					fprintf(stderr,"Failed to sync between server and client between different msg sizes\n");
					return FAILURE;
				}
			}

			print_report_bw(&user_param1,&my_bw_rep1);

			if (user_param1.duplex) {
				xchg_bw_reports(&user_comm1, &my_bw_rep1,&rem_bw_rep1,atof(user_param1.rem_version));
				print_full_bw_report(&user_param1, &my_bw_rep1, &rem_bw_rep1);
			}
		}

	} else if (user_param1.test_method == RUN_REGULAR) {

    printf("Run Regular\n");
		ctx_set_send_wqes(&ctx1,&user_param1,rem_dest1);

		if (user_param1.verb != SEND) {

			if (user_param1.perform_warm_up) {
				if(perform_warm_up(&ctx1, &user_param1)) {
					fprintf(stderr, "Problems with warm up\n");
					return FAILURE;
				}
			}
		}
    
    if(user_param1.duplex) {
			if (ctx_hand_shake(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
				fprintf(stderr,"Failed to sync between server and client between different msg sizes\n");
				return FAILURE;
			}
		}
   
   	ctx_set_send_wqes(&ctx2,&user_param2,rem_dest2);

		if (user_param2.verb != SEND) {

			if (user_param2.perform_warm_up) {
				if(perform_warm_up(&ctx2, &user_param2)) {
					fprintf(stderr, "Problems with warm up\n");
					return FAILURE;
				}
			}
		}
    
    if(user_param2.duplex) {
			if (ctx_hand_shake(&user_comm2,&my_dest2[0],&rem_dest2[0])) {
				fprintf(stderr,"Failed to sync between server and client between different msg sizes\n");
				return FAILURE;
			}
		}
   
       
    printf("Run BW\n");
    
	struct bw_args *args1 = (struct bw_args*)malloc(sizeof(struct bw_args));
	args1->ctx = &ctx1;
    args1->user_param = &user_param1;

	struct bw_args *args2 = (struct bw_args*)malloc(sizeof(struct bw_args));
	args2->ctx = &ctx2;
    args2->user_param = &user_param2;

	pthread_create(&dev1_thread,NULL,&run_iter_bw_mt,(int *)args1);
    pthread_create(&dev2_thread,NULL,&run_iter_bw_mt,(int *)args2);
    pthread_join(dev1_thread,NULL);
    pthread_join(dev2_thread,NULL); 

/*
   	if(run_iter_bw(&ctx1,&user_param1)) {
			fprintf(stderr," Failed to complete run_iter_bw function successfully\n");
			return FAILURE;
		}
   
   	if(run_iter_bw(&ctx2,&user_param2)) {
			fprintf(stderr," Failed to complete run_iter_bw function successfully\n");
			return FAILURE;
		}
*/
		print_report_bw(&user_param1,&my_bw_rep1);
		print_report_bw(&user_param2,&my_bw_rep2);
    
    
		if (user_param1.duplex) {
			xchg_bw_reports(&user_comm1, &my_bw_rep1,&rem_bw_rep1,atof(user_param1.rem_version));
			print_full_bw_report(&user_param1, &my_bw_rep1, &rem_bw_rep1);
		}

		if (user_param1.report_both && user_param1.duplex) {
			printf(RESULT_LINE);
			printf("\n Local results: \n");
			printf(RESULT_LINE);
			printf((user_param1.report_fmt == MBS ? RESULT_FMT : RESULT_FMT_G));
			printf((user_param1.cpu_util_data.enable ? RESULT_EXT_CPU_UTIL : RESULT_EXT));
			print_full_bw_report(&user_param1, &my_bw_rep1, NULL);
			printf(RESULT_LINE);

			printf("\n Remote results: \n");
			printf(RESULT_LINE);
			printf((user_param1.report_fmt == MBS ? RESULT_FMT : RESULT_FMT_G));
			printf((user_param1.cpu_util_data.enable ? RESULT_EXT_CPU_UTIL : RESULT_EXT));
			print_full_bw_report(&user_param1, &rem_bw_rep1, NULL);
    }
		if (user_param2.duplex) {
			xchg_bw_reports(&user_comm2, &my_bw_rep2,&rem_bw_rep2,atof(user_param2.rem_version));
			print_full_bw_report(&user_param2, &my_bw_rep2, &rem_bw_rep2);
		}

		if (user_param2.report_both && user_param2.duplex) {
			printf(RESULT_LINE);
			printf("\n Local results: \n");
			printf(RESULT_LINE);
			printf((user_param2.report_fmt == MBS ? RESULT_FMT : RESULT_FMT_G));
			printf((user_param2.cpu_util_data.enable ? RESULT_EXT_CPU_UTIL : RESULT_EXT));
			print_full_bw_report(&user_param2, &my_bw_rep2, NULL);
			printf(RESULT_LINE);

			printf("\n Remote results: \n");
			printf(RESULT_LINE);
			printf((user_param2.report_fmt == MBS ? RESULT_FMT : RESULT_FMT_G));
			printf((user_param2.cpu_util_data.enable ? RESULT_EXT_CPU_UTIL : RESULT_EXT));
			print_full_bw_report(&user_param2, &rem_bw_rep2, NULL);
      

		}
   printf("\n RunRegular2 end \n");
   
	} else if (user_param1.test_method == RUN_INFINITELY) {

		ctx_set_send_wqes(&ctx1,&user_param1,rem_dest1);
		ctx_set_send_wqes(&ctx2,&user_param2,rem_dest2);

		//if(run_iter_bw_infinitely_dual(&ctx1,&user_param1,&ctx2,&user_param2)) {
   if(run_iter_bw_infinitely(&ctx1,&user_param1)) {
			fprintf(stderr," Error occurred while running infinitely! aborting ...\n");
			return FAILURE;
		}
	}

	if (user_param1.output == FULL_VERBOSITY) {
    printf("\n Full Verbosity\n");
		if (user_param1.report_per_port){
			printf("\n Result Line per port\n");
			printf(RESULT_LINE_PER_PORT);

      }
		else{
			printf("\n Result Line\n");
			printf(RESULT_LINE);
      }
	}
    
	printf("half duplex clien1t/n");
	/* For half duplex tests, server just waits for client to exit */
	if (user_param1.machine == CLIENT && !user_param1.duplex) {
    printf("half duplex client/n");
		if (ctx_hand_shake(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
			fprintf(stderr," Failed to exchange data between server and clients\n");
			return FAILURE;
		}

		xchg_bw_reports(&user_comm1, &my_bw_rep1,&rem_bw_rep1,atof(user_param1.rem_version));
	}
 
 	/* For half duplex tests, server just waits for client to exit */
	if (user_param2.machine == CLIENT && !user_param2.duplex) {
    printf("half duplex client2/n");
		if (ctx_hand_shake(&user_comm2,&my_dest2[0],&rem_dest2[0])) {
			fprintf(stderr," Failed to exchange data between server and clients\n");
			return FAILURE;
		}

		xchg_bw_reports(&user_comm2, &my_bw_rep2,&rem_bw_rep2,atof(user_param2.rem_version));
	}

	printf("Clsoing Connection1/n");
	/* Closing connection. */
	if (ctx_close_connection(&user_comm1,&my_dest1[0],&rem_dest1[0])) {
		fprintf(stderr,"Failed to close connection between server and client\n");
		return FAILURE;
	}
 
	printf("Clsoing Connection2/n");
 	/* Closing connection. */
	if (ctx_close_connection(&user_comm2,&my_dest2[0],&rem_dest2[0])) {
		fprintf(stderr,"Failed to close connection between server and client\n");
		return FAILURE;
	}

	if (!user_param1.is_bw_limit_passed && (user_param1.is_limit_bw == ON ) ) {
		fprintf(stderr,"Error: BW result is below bw limit\n");
		return FAILURE;
	}

	if (!user_param1.is_msgrate_limit_passed && (user_param1.is_limit_bw == ON )) {
		fprintf(stderr,"Error: Msg rate  is below msg_rate limit\n");
		return FAILURE;
	}

	free(my_dest1);
	free(rem_dest1);

	if (user_param1.work_rdma_cm == ON) {
		if (destroy_ctx(&ctx1,&user_param1)) {
			fprintf(stderr, "Failed to destroy resources\n");
			return FAILURE;
		}

		user_comm1.rdma_params->work_rdma_cm = OFF;
		return destroy_ctx(user_comm1.rdma_ctx,user_comm1.rdma_params);
	}

	return destroy_ctx(&ctx1,&user_param1);
}

