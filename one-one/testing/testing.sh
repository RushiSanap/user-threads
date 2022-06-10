echo "-------------------------------------------"
echo "testing for thread create"
gcc ../spin_lock.c ../queue.c ../mythread.c test1.c
./a.out
echo "-------------------------------------------"
echo "testing for thread join"
gcc ../spin_lock.c ../queue.c ../mythread.c test2.c
./a.out
echo "-------------------------------------------"
echo "testing for invalid argument for start routine"
gcc ../spin_lock.c ../queue.c ../mythread.c test3.c
./a.out
echo "-------------------------------------------"
echo "testing for invalid argument of thread_id in thread join"
gcc ../spin_lock.c ../queue.c ../mythread.c test4.c
./a.out
echo "-------------------------------------------"
echo "testing for multiple thread creation"
gcc ../spin_lock.c ../queue.c ../mythread.c test5.c
./a.out
echo "-------------------------------------------"
echo "testing for multiple thread creation"
gcc ../spin_lock.c ../queue.c ../mythread.c test6.c
./a.out
echo "-------------------------------------------"
echo "testing for spinlock"
gcc ../spin_lock.c ../queue.c ../mythread.c test7.c
./a.out
echo "-------------------------------------------"
echo "testing for return value"
gcc ../spin_lock.c ../queue.c ../mythread.c test8.c
./a.out
echo "-------------------------------------------"
# gcc ../spin_lock.c ../queue.c ../mythread.c test9.c
# gcc ../spin_lock.c ../queue.c ../mythread.c test10.c








