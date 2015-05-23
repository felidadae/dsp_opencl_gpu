
//Arguments Description
//pir_R, pir_I                      pir == partitioned impulse response, ganged (one pir channel after another) in frequency domain
//fdl_R, fdl_I                      delay lines in frequency domain
//accu_R, accu_I                    ganged channels windows being result of complex multiplying and adding, in frequency domain
// _2B info[0]                      part size, length of one channel window
// _C info[1]                       number of channels in fdl
// _P info[2]                       number of ir parts  >>>>>per one channel<<<<<
// pir_C info[3]                    number of channels of ir
// FDL_LINE info[4]                 cursor at the newest fdl line (that which has been just inserted)
//.

__kernel
void complexMultiplyAdd (__global float*    pir_R,
                         __global float*    pir_I,
                         __global float*    fdl_R,
                         __global float*    fdl_I,
                         __global float*   accu_R,
                         __global float*   accu_I,
                         __global unsigned int* info )
{
    //get info to private memory
    unsigned int _2B                = info[0];
    unsigned int _C                 = info[1];
    unsigned int _P                 = info[2];
    unsigned int pir_C              = info[3];
    unsigned int fdl_lastest_line   = info[4];
    //.
    
    unsigned int idx = get_global_id(0);
    
    //As if sample [channNum][sampleNum]
    unsigned int channNum = idx / _2B;
    unsigned int sampleNum= idx % _2B;
    //.
    
    //pir index
    unsigned int pir_Idx        = sampleNum % (pir_C * _2B);    //in case if (pir_C != _C)
    //.
    
    //fdl index
    unsigned int fdl_Idx_base   = channNum * _2B +  sampleNum ;
    unsigned int fdl_cursor     = fdl_lastest_line;
    unsigned int fdl_Idx        = (fdl_cursor * _2B * _C) + fdl_Idx_base;
    //.
    
    //private accu
    float accu_R_local = 0,            accu_I_local = 0;
    //.
    
    //complex multiply add
    for(unsigned int partNum = 0; partNum < _P; ++partNum) {
        
        //copy to private memory
        float pir_R_local = pir_R[pir_Idx];
        float pir_I_local = pir_I[pir_Idx];
        //---
        float fdl_R_local = fdl_R[fdl_Idx];
        float fdl_I_local = fdl_I[fdl_Idx];
        //.
        
        //complex multiply
        //(a, b)*(c,d)= (ac - bd, bc + ad)
        accu_R_local += fdl_R_local * pir_R_local - fdl_I_local * pir_I_local;
        accu_I_local += fdl_I_local * pir_R_local + fdl_R_local * pir_I_local;
        //.
        
        //increment indexes
        pir_Idx += _2B * _C;
        fdl_cursor  = (fdl_cursor - 1 + _P) % _P;
        fdl_Idx  = (fdl_cursor * _2B * _C) + fdl_Idx_base;
        //.
        
    }//.
    
    //writing result to global memory
    accu_R[idx] = accu_R_local;
    accu_I[idx] = accu_I_local;
    //.
}
