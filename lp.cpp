#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

int
main (int argc, char **argv)
{
	int N = 5000;
	double M = 100000;
  double** w = new double*[N];
	for (int i = 0; i < N; i++){
		w[i] = new double[N];
		for (int j = 0; j < N; j++){
			if (i == j){
				w[i][j] = i*j;
			} 
			else {
				w[i][j] = i+j;
			}
		}
	} 
	
	IloEnv   env;
   try {
			IloModel model(env);
      
      IloNumVarArray xa(env);
			IloNumVarArray xi(env);
			
			for (int i = 0; i < N; i++){
				xa.add(IloNumVar(env));
				xi.add(IloNumVar(env));
			}
		
			IloNumExpr obj = xa[0] + xi[0];	
			
			for (int i = 1; i < N; i++){
				
				obj = obj + xa[i];
				obj = obj + xi[i];
			}	
			
      IloRangeArray con(env);
			for (int i = 0; i < N; i++){
				for (int j = 0; j < N; j++){
					con.add(xa[i] + xi[j] >= w[i][j]);
				}
			}
			
			model.add(con);
   		model.add(IloMinimize(env, obj));
			// populate model 
 
      IloCplex cplex(model);

      if ( !cplex.solve() ) {
         env.error() << "Failed to optimize LP" << endl;
         throw(-1);
      }
      env.out() << "Solution value  = " << cplex.getObjValue() << endl;
   }
   catch (IloException& e) {
      cerr << "Concert exception caught: " << e << endl;
   }
   catch (...) {
      cerr << "Unknown exception caught" << endl;
   }

   env.end();

   return 0;
}
