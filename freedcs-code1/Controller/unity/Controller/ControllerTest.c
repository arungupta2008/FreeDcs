/**
 * Project: FreeDCS: A Free Distributed Control System under GPL license.
 * @author Robert Perez, Montevideo-Uruguay, Copyright (C) 2007-2013
 * @see The GNU Public License (GPL)
 *
 * This file contains the code that handles common station communication.
 * It contains code to receive a new client (engineering or operator station) and forks a new process
 * to handle that client.
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "../unity.framework/extras/fixture/src/unity_fixture.h"
#include "statcom.h"

//#include "unity_fixture.h"

static struct used_port test_used_ports[100];

TEST_GROUP(Controller);

TEST_SETUP(Controller)
{
    ext_max_clients = 100;
    int i;
    for(i=0; i<100; i++)
    {
        test_used_ports[i].childpid = 0;
        test_used_ports[i].ports = 0;
    }
}

TEST_TEAR_DOWN(Controller)
{
}

TEST(Controller, ReleaseDeadConnectionNullUsedPortArray)
{
    test_used_ports[5].childpid = 33;
	test_used_ports[5].ports = 7;
    TEST_ASSERT_EQUAL(-1, release_dead_connection_entry(33, NULL));
    TEST_ASSERT_EQUAL(33, test_used_ports[5].childpid);
    TEST_ASSERT_EQUAL(7, test_used_ports[5].ports);
}

TEST(Controller, ReleaseDeadConnectionChildPIDZero)
{
    test_used_ports[5].childpid = 33;
	test_used_ports[5].ports = 7;
    TEST_ASSERT_EQUAL(-1, release_dead_connection_entry(0, test_used_ports));
    TEST_ASSERT_EQUAL(33, test_used_ports[5].childpid);
    TEST_ASSERT_EQUAL(7, test_used_ports[5].ports);
}

TEST(Controller, ReleaseDeadConnectionChildPIDNegative)
{
    test_used_ports[5].childpid = 33;
	test_used_ports[5].ports = 7;
    TEST_ASSERT_EQUAL(-1, release_dead_connection_entry(-15, test_used_ports));
    TEST_ASSERT_EQUAL(33, test_used_ports[5].childpid);
    TEST_ASSERT_EQUAL(7, test_used_ports[5].ports);
}


TEST(Controller, ReleaseDeadConnectionEntryFound)
{
    test_used_ports[5].childpid = 33;
	test_used_ports[5].ports = 1;
    TEST_ASSERT_EQUAL(0, release_dead_connection_entry(33, test_used_ports));
    TEST_ASSERT_EQUAL(0, test_used_ports[5].childpid);
    TEST_ASSERT_EQUAL(0, test_used_ports[5].ports);
}

TEST(Controller, ReleaseDeadConnectionEntryNotFound)
{
    test_used_ports[5].childpid = 99;
	test_used_ports[5].ports = 1;
    TEST_ASSERT_EQUAL(-1, release_dead_connection_entry(39, test_used_ports));
    TEST_ASSERT_EQUAL(99, test_used_ports[5].childpid);
    TEST_ASSERT_EQUAL(1, test_used_ports[5].ports);
}

int get_socket()
{
    int mysocket;
    mysocket = socket(PF_INET, SOCK_STREAM, 0);
    return mysocket;
}

TEST(Controller, CloseConnectionNullModuleName)
{
    SSL *ssl=NULL;
    SSL_CTX *ctx=NULL;
    char *module_name = (char*)0;

    TEST_ASSERT_EQUAL(0,closeconnection(get_socket(), ssl, ctx, module_name));
}


TEST(Controller, GetNewPortAllUsed)
{
    int i;
    for(i=0; i<100; i++) ///make them all used
    {
        test_used_ports[i].childpid = i;
        test_used_ports[i].ports = 1;
    }

    TEST_ASSERT_EQUAL(NO_FREE_CONN,get_new_port(get_socket(), test_used_ports));
}

TEST(Controller, GetNewPortLastOneFree)
{
    int i;
    for(i=0; i<99; i++) ///make them all used but the last
    {
        test_used_ports[i].childpid = i;
        test_used_ports[i].ports = 1;
    }

    TEST_ASSERT_EQUAL(99,get_new_port(get_socket(), test_used_ports));
}



///fake functions for xenomai!!

int rt_task_bind(RT_TASK *task, const char *name, RTIME timeout)
{
    return 0;
}

int rt_task_shadow(RT_TASK *task, const char *name,  int prio, int mode)
{
    return 0;
}
