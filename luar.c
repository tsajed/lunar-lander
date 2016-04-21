#include "struct.h"
#include "functions.h"



const char exec[] = "java -jar Sketchpad.jar";



int main (int argc, char *argv[]) {

    landerdata *data = malloc(1 * sizeof(struct landerdata));

    char arg[10][256] ;
	
    if (argc >= 7 && argc < 9) {
    
        for (int i = 0; i < argc; i++) {
        
            strcpy(arg[i], argv[i]);
            data->args[i] = arg[i];
        }
    }
    
    else {
    
        fprintf(stderr, "Error : invalid number of arguments\n");
        exit(EXIT_FAILURE);
    }
       
    FILE *fp;
    if (strcmp(argv[argc -1], "-i") == 0)          
      fp = Check(data, fp);

    else if(argc == 7)
      fp = Check2(data,fp);

    else {
      printf("Error :bad %s, or good option but no value.\n", argv[7]);
      exit(EXIT_FAILURE);
    }
    
    FILE *pipe = popen(exec, "w");
    if (pipe == NULL) 
        fprintf(stderr, "Error : Invalid Pipe\n");
	
    data->p = pipe;
    data->f = fp;

    if (strcmp(argv[argc-1], "-i") == 0)
      InitTimer(data);

    else 
      ListenKey(data);

    fprintf(pipe, "end\n");
    pclose(pipe);
    endwin();
     
    return EXIT_SUCCESS;
}



FILE *Check (landerdata *data, FILE *fp) {

    int m = 0;
    
    for (int i = 1; i < 6; i++) {
    
	if (strcmp(data->args[i], "-g") == 0) {
                
	   int k=sscanf(data->args[i+1], "%lf", &data->gravity);
           if (k<0) {
             printf("Error : -g [option], [option] is not a number\n");
             exit(EXIT_FAILURE);
           }

           if (data->gravity <0 && data->gravity>20){
             printf("Error :gravity < 0, > 20 not allowed\n");
             exit(EXIT_FAILURE);
           }
	    m++;
        }
        
        else if (strcmp(data->args[i], "-t") == 0) {
            
            int k= sscanf(data->args[i+1], "%lf", &data->thrust);
            if (k<0) {
              printf("Error : -t [option], [option] not a number\n");
              exit(EXIT_FAILURE);
            }

            if (data->thrust >0 && data->thrust < -20){
             printf("Error :thrust > 0, < -20 not allowed\n");
             exit(EXIT_FAILURE);
           }
              
            m++;
        }
            
        else if (strcmp(data->args[i], "-f") == 0) {
            
            fp = fopen(data->args[i+1], "r");
            if (fp == NULL) 
                fprintf(stderr, "Error : Invalid file\n");
                    
            m++;
        }
                        
    }
        
    if ( m != 3) {
            
	fprintf(stderr,
        "Error : Invalid arguments or repeated arguments in command line\n");
	exit(EXIT_FAILURE);
    }
            
    return fp;

}


int InitMap(landerdata *data) {
	    
    char tokens[256];
    int k = 0;
    
    while (!feof(data->f)) {
    
        char *p = fgets(tokens, 256, data->f);
        if (p == NULL) 
            break;						    		
        
        sscanf(tokens, "%ld %ld", &data->m_coord[k+1], &data->m_coord[k+2]);

        k = k+2;
    }
    
    data->m_coord[0] = k;
    
    for (int i = 1; i <= (data->m_coord[0]-2); i = i + 2) {
	
	fprintf(data->p, "drawSegment %ld %ld %ld %ld\n", data->m_coord[i],
		data->m_coord[i+1], data->m_coord[i+2],
		data->m_coord[i+3]);
    }
    
    fclose(data->f);	 
}


double FindCentre(landerdata *data) {
	
    double m[4];
    int j = 0;
    while (j < 2) {
	
    	m[j] = data->l_coord[1];
	m[j+2] = data->l_coord[2];
	j++;
    }
    for (int i = 1; i <= data->l_coord[0]; i = i + 2) {
	
	if (m[0] > data->l_coord[i]) {
            m[0] = data->l_coord[i];
	}
	
	if (m[1] < data->l_coord[i]) {
	    m[1] = data->l_coord[i];
	}
	
	if (m[2] > data->l_coord[i+1]) {
	    m[2] = data->l_coord[i+1];
	}
	
	if (m[3] < data->l_coord[i+1]) {
	    m[3] = data->l_coord[i+1];
	}
    }
    
    data->centre_x = (m[0] + m[1])/2;
    data->centre_y = (m[2] + m[3])/2;
    
    return m[0];
}


FILE *Check2(landerdata *data, FILE *fp) {

  int m = 0;
  char tokens[10][256];
    
    for (int i = 1; i < 6; i++) {
    
	if (strcmp(data->args[i], "-g") == 0) {
                
         int k = sscanf(data->args[i+1], "%s", tokens[0]);

         if (k<0) {
           printf("Error :bad -g, or good option but no value.\n");
           exit(EXIT_FAILURE);
         }
          data->gravity = strtod(tokens[0], NULL);

           if (data->gravity <0 && data->gravity>20){
             printf("Error :gravity < 0, > 20 not allowed\n");
             exit(EXIT_FAILURE);
           }
	    m++;
            printf("%d", m);
        }
        
        else if (strcmp(data->args[i], "-t") == 0) {
            
            int k = sscanf(data->args[i+1], "%s", tokens[1]);

            if (k<0) {
              printf("Error :bad -t, or good option but no value.\n");
              exit(EXIT_FAILURE);
            }

            data->thrust = strtod(tokens[1], NULL);
           
            if (data->thrust >0 && data->thrust < -20){
             printf("Error :thrust > 0, <- 20 not allowed\n");
             exit(EXIT_FAILURE);
           }
              
            m++;
              printf("%d", m);
        }
            
        else if (strcmp(data->args[i], "-f") == 0) {
            
            fp = fopen(data->args[i+1], "r");
            
            if (fp == NULL)  {
                fprintf(stderr, "Error : Invalid file\n");
                exit(EXIT_FAILURE);
            }
            m++;
              printf("%d", m);
        }
                        
    }
        
    if ( m != 3) {
            
	fprintf(stderr,
        "Error : Invalid arguments or repeated arguments in command line\n");
	exit(EXIT_FAILURE);
    }
            
    return fp;

}
