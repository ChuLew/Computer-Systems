if(pid==0){ //child1
        pid = fork();
        if(pid==0){//child1 of child1
            print("I am child 1 of child 1");
            int max1 = max(firstHalf,count);
            sigqueue(mainPid,SIGUSR1,(const union sigval) max1);
        }
        else{//child1
            pid = fork();
            if(pid==0){//child 2 of child 1
                print("I am child 2 of child 1");
                int min1 = min(firstHalf,count);
		sigqueue(mainPid,SIGUSR1,(const union sigval) min1);
            }
            else{//child 1 waits on children
                print("I am child 1");
                int sum1 = sum(firstHalf,count);
                sigqueue(mainPid,SIGUSR2,(const union sigval) sum1);
                while ((wpid = wait(&status)) > 0);     
                print("child1 has waited on its 2 children");
            }
        }
    }
    else{ //parent
        pid = fork();
        
        if (pid==0){//child2
            pid = fork();
            if(pid==0){//child1 of child 2
                print("I am child 1 of child 2");
                int max2 = max(secondHalf,count);
		sigqueue(mainPid,SIGUSR1,(const union sigval) max2);
            }
            else{//child2 
                pid = fork();
                if(pid==0){//child2 of child 2
                   print("I am child 2 of child 2");
                   int min2 = min(secondHalf,count);
                   sigqueue(mainPid,SIGUSR1,(const union sigval) min2);
                }
                else{//child 2 waits on children 
                     print("I am child 2");
                    int sum2 = sum(secondHalf,count);
                    sigqueue(mainPid,SIGUSR2,(const union sigval) sum2);
                    while ((wpid = wait(&status)) > 0);
                    print("child2 has waited on its 2 children");
                }
            }
        }
        else{//parent
            while ((wpid = wait(&status)) > 0);
            print("Main has waited on its 2 children");
            int t,maximum=minmax[0],minimum=minmax[0],finsum;
            for(t=0;t<4;t++){
                printf("In min max is: %d\n",minmax[t]);
                    if(minmax[t]>maximum){
                            maximum=minmax[t];
                    }
            }
            for(t=0;t<4;t++){
                    if(minmax[t]<minimum&&minmax[t]!=0){
                            minimum=minmax[t];
                    }
            }
            finsum=totsum[0]+totsum[1];
            printf("The maximum value is %d\n",maximum);
            printf("The minimum value is %d\n",minimum);
            printf("The sum value is %d\n",finsum);
        }
    }

///8waeuy nrapoweisufdoipasudfoipasudfoipjdsoifpjldsfkdskj





    mainPid = getpid();
    pid_t pid,wpid,pidchild1,pidchild2;
    int status;
    pidchild1 = fork();
    if(pidchild1==0){ //child1
        pid = fork();
        if(pid==0){//child1 of child1
            print("I am child 1 of child 1");            
        }
        else{//child1
            pid = fork();
            if(pid==0){//child 2 of child 1
                print("I am child 2 of child 1");             
            }
            else{//child1 //child 1 waits on children
                print("I am child 1");        
                while ((wpid = wait(&status)) > 0);     
                print("child1 has waited on its 2 children");
            }
        }
    }
    else{ //parent
        pidchild2 = fork();       
        if (pidchild2==0){//child2
            pid = fork();
            if(pid==0){//child1 of child 2
                print("I am child 1 of child 2");         
            }
            else{//child2 
                pid = fork();
                if(pid==0){//child2 of child 2
                   print("I am child 2 of child 2");                
                }
                else{//child 2 //child 2 waits on children                 
                    while ((wpid = wait(&status)) > 0);
                    print("child2 has waited on its 2 children");
                }
            }
        }
        else{//parent
            while ((wpid = wait(&status)) > 0); 
            print("HAS WAITED ON BOTH MAIN CHILDREN");
        }
       
    }
    
    asdfakdsjfalk;sdjfalk;sjdflk;ajsdlkf;ajslkd;jfal;ksdjfalk;dsjflk;dsj
    
    if(pid==0){ //child1
        pid = fork();
        if(pid==0){//child1 of child1
            print("I am child 1 of child 1");
        }
        else{//child1
            pid = fork();
            if(pid==0){//child 2 of child 1
                print("I am child 2 of child 1");              
            }
            else{//child 1 waits on children
                print("I am child 1");           
            }
        }
    }
    else{ //parent
        pid = fork();       
        if (pid==0){//child2
            pid = fork();
            if(pid==0){//child1 of child 2
                print("I am child 1 of child 2");             
            }
            else{//child2 
                pid = fork();
                if(pid==0){//child2 of child 2
                   print("I am child 2 of child 2");              
                }
                else{//child 2 waits on children 
                     print("I am child 2");
                    int sum2 = sum(secondHalf,count);                 
                }
            }
        }
        else{//parent
           //i want to wait here for children and grandchildren process
        }
    }