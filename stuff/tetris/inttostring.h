int pow(int var, unsigned char exp){
	
	if(exp == 0){
		return 1;
	}
	if(exp == 1){
		return var;
	}
	int kek = var;
	for(unsigned char i = 1;i < exp;i++){
		var *= kek;
	}
	return var;
}

char * slow_itos(int con){
	char * ans;//returns pointer to converted string when done (see below)
	unsigned char neg = 0; //check for negatie number
	int a = 2;//string length
	int mag = 10;//digit factor
	int hold = 0;//work variable	
	if((con & 0x80000000) != 0){
		neg = 1;
	}
	
	con = abs(con);
	
	if(con > 9){
		for(;a<11;a++){
		
			mag = pow(10,a);
			
			if(con < mag){
				break;
			}
			
			if(con == mag){
				//power of 10 case
				a++;
				ans = (char *)malloc((a+1)+neg);//allocate
				ans[a+neg] = '\0'; //puts null at string end
				ans[0+neg] = '1';
				for(int i = 1;i < a;i++){
					ans[i+neg] = '0';
				}
				if(neg == 1){
					ans[0] = '-';
				}
				return ans;
			}
			
		}
	}else{
		//base case for single digits
		ans = (char *)malloc(2);
	    ans[0] = (char)(con + 48);
		ans[1] = '\0';
		return ans;
	}
	
	ans = (char *)malloc((a+1)+neg);//allocate
	ans[a+neg] = '\0'; //puts null at string end
	ans += ((a-1)+neg);//moves pointer to end of string
	for(unsigned char i = 0; i < a-1; i++){
		mag = pow(10, i);
		hold = (con - ((mag * 10) * floor((float)con / (float)(mag * 10)))); //pop off next digit
		//pop(x):
		*ans = (char)((hold / mag) + 48);//add digit to string
		ans--;//next char
		//end pop
		con -= hold;//ditto
	}
	//pop(x):
	mag = pow(10, (a-1));
	*ans = (char)((con / mag) + 48);//add digit to string
	
	//end pop
	//pop(sign): 
	if(neg){
		ans--;
		*ans = '-';//add '-' to string
	}
	//end pop
	return ans;
}
