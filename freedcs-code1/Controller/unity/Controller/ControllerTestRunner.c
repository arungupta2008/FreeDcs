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

TEST_GROUP_RUNNER(Controller)
{
    RUN_TEST_CASE(Controller, ReleaseDeadConnectionEntryFound);
    RUN_TEST_CASE(Controller, ReleaseDeadConnectionEntryNotFound);
    RUN_TEST_CASE(Controller, ReleaseDeadConnectionNullUsedPortArray);
    RUN_TEST_CASE(Controller, ReleaseDeadConnectionChildPIDZero);

    RUN_TEST_CASE(Controller, ReleaseDeadConnectionChildPIDNegative);
    RUN_TEST_CASE(Controller, CloseConnectionNullModuleName);
    RUN_TEST_CASE(Controller, GetNewPortAllUsed);
    RUN_TEST_CASE(Controller, GetNewPortLastOneFree);


//RUN_TEST_CASE(Controller, );
//RUN_TEST_CASE(Controller, );
//RUN_TEST_CASE(Controller, );
//RUN_TEST_CASE(Controller, );
//RUN_TEST_CASE(Controller, );
//RUN_TEST_CASE(Controller, );
//RUN_TEST_CASE(Controller, );
//RUN_TEST_CASE(Controller, );

}
