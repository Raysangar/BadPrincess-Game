#ifndef __AI_ResourceServer_H
#define __AI_ResourceServer_H

namespace AI 
{
	//Enum Global
	enum ResourceType{
		Wood,
		Iron,
		Stone,
		none,
	};

	class ResourceServer{
	public:
		int getResource(ResourceType type);

		void addResource(ResourceType type, int quantity);

		static ResourceServer* getInstance();

		static void Release();
		/*
		Function that get the entity resource cost from lua and check if there are enough resource available
		@params fieldname entit type name that correspond to lua table object
		@return true if there are enough resource available
		*/
		bool checkResourceAvailability(const char* fieldName);
		/*
		Function that get the entity resource cost from lua and check if there are enough resource available
		@params fieldname entity type name that correspond to lua table object
		@params percentage float from 0 to 1 that represent the quantity of the total entity cost must be avaliable
		@return true if there are enough resource available
		*/
		bool checkResourceAvailability(const char* fieldName,float percetage, bool showWarning = false);
		/*
		Function that get the entity resource cost from lua and substract that cost to the resource
		@params fieldname entit type name that correspond to lua table object
		@return true if there are enough resource available
		*/
		void useResource(const char* fieldName);
		/*
		Function that get the entity resource cost from lua and substract that cost to the resource
		@params fieldname entity type name that correspond to lua table object
		@params percentage float from 0 to 1 that represent the quantity of the total entity cost must be substract
		@return true if there are enough resource available
		*/
		void useResource(const char* fieldName,float percetage);

		int getRequiredQuantity(const char* fieldName,char* type);

		/*
		Reset all variable referred to a single instance of game
		*/
		void reset();
	private:
		ResourceServer(int metal, int stone, int wood);

		void init();

		~ResourceServer();

		static ResourceServer* instance;

		int resources[3];
	};
}

#endif