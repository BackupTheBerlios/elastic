package gotoexample;

import basic;

private function init_task(task)
{
	if (task == "task3")
		return @false;
	return @true;
}

private function init_all_tasks(tasks)
{
	local task;

    for (task in tasks)
    {
        if (! init_task( task )) goto ouch;
        basic.print( "Task ", task, " OK\n" );
    }

    basic.print( "All tasks OK\n" );
    return @true;

    ouch:
    basic.print( "Ouch: initialization failed\n" );
    return @false;
}

private function main()
{
	local tasks = #["task1", "task2", "task3", "task4"];

	init_all_tasks( tasks );
}

main();
