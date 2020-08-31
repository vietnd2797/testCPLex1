#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
IloInt nbVehicles, nbItems; 
IloNumArray num_of_items, cost;
IloNumArray2 capacity; 
void defineData(IloEnv env)
{
	nbVehicles = 3; 
	nbItems = 3; 
	num_of_items = IloNumArray(env, nbItems, 1000, 800, 20);
	cost = IloNumArray(env, nbVehicles, 10000, 1100, 10000);
	capacity = IloNumArray2(env, nbItems);
	capacity[0] = IloNumArray(env, nbVehicles, 100, 1, 10);
	capacity[1] = IloNumArray(env, nbVehicles, 1, 4, 50);
	capacity[2] = IloNumArray(env, nbVehicles, 0, 2, 1);
}
int main()
{
	IloEnv env;
	try
	{
		IloInt i;
		defineData(env);
		IloModel model(env);

		//Decision variables: Number of vehicles needed to transport items.
		IloNumVarArray x(env, nbVehicles, 0, IloInfinity, ILOINT);

		//Objective function
		model.add(IloMinimize(env, IloScalProd(cost, x))); 


		//Constraints
		for (i = 0; i < nbItems; i++)
		{
			//IloIntExpr expr(env);
			//for (j = 0; j < nbVehicles; j++)
			//{
				//expr += capacity[i][j] * x[j];
			//}
			model.add(IloScalProd(capacity[i], x) >= num_of_items[i]); 
			//expr.end();
		}

		//Solve
		IloCplex cplex(model);
		cplex.setOut(env.getNullStream());
		cplex.setWarning(env.getNullStream());
		if (!cplex.solve()) {
			env.error() << "Failed to optimize the problem." << endl;
			throw(-1);
		}
		if (cplex.getStatus() == IloAlgorithm::Infeasible)
		{
			env.out() << "No solution" << endl;
		}
		env.out() << "Solution status: " << cplex.getStatus() << endl;

		//Results
		env.out() << "Optimal cost:" << cplex.getObjValue() << endl;
		env.out() << "Number of vehicles needed to transport items:" << endl;
		for (i = 0; i < nbVehicles; i++)
		{
			env.out() << cplex.getValue(x[i]) << endl; 
		}

	}
	catch (IloException& ex) 
	{
		cerr << "Error: " << ex << endl;
	}
	catch (...) 
	{
		cerr << "Error" << endl;
	}
	env.end();
	return 0;
}