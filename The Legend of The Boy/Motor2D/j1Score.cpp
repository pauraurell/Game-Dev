#include "j1Score.h"
#include "j1App.h"
#include "j1UI.h"
#include "j1Render.h"

j1Score::j1Score()
{

}

j1Score::~j1Score()
{}

bool j1Score::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Score::Start()
{
	draw = false;

	

	score_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 140,90 }, 25, false, { 0,0,0,0 }, { 0,0 }, "SCORE:", this);
	timeScore_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 120,180 }, 20, false, { 0,0,0,0 }, { 0,0 }, nullptr, this);
	coinsScore_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 120,140 }, 20, false, { 0,0,0,0 }, { 0,0 }, nullptr, this);
	enemiesScore_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 120,160 }, 20, false, { 0,0,0,0 }, { 0,0 }, nullptr, this);
	lifesScore_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 120,120 }, 20, false, { 0,0,0,0 }, { 0,0 }, nullptr, this);
	totalScore_label = App->ui->Add_UIelement(TYPE_UI::UI_LABEL, nullptr, { 120,210 }, 22, false, { 0,0,0,0 }, { 0,0 }, nullptr, this);

	return true;
}

bool j1Score::PreUpdate()
{
	return true;
}

bool j1Score::Update(float dt)
{
	background.x = App->render->camera.x * (-1 / 2) + 200;
	background.y = App->render->camera.y * (-1 / 2) + 150;
	background.w = 630;
	background.h = 350;
	
	return true;
}

bool j1Score::PostUpdate()
{
	Draw();

	return true;
}

bool j1Score::CleanUp()
{
	for (p2List_item<UIelement*>* item = App->ui->UIelements.start; item; item = item->next)
	{
		if (item->data == score_label || item->data == timeScore_label || item->data == coinsScore_label || item->data == totalScore_label || item->data == enemiesScore_label || item->data == lifesScore_label)
		{ item->data->CleanUp(); }
	}

	return true;
}

bool j1Score::Draw()
{
	if (draw == true)
	{
		score_label->enabled = true;
		timeScore_label->enabled = true;
		coinsScore_label->enabled = true;
		enemiesScore_label->enabled = true;
		lifesScore_label->enabled = true;
		totalScore_label->enabled = true;

		SDL_SetRenderDrawColor(App->render->renderer, 255, 255, 255, 200);
		SDL_RenderFillRect(App->render->renderer, &background);

		lifesScore = App->ui->pLife * 50;
		p2SString* string = new p2SString("Lifes: %i x 50 -> %i", App->ui->pLife, lifesScore);
		lifesScore_label->text = string->GetString();

		coinsScore = coins * 10;
		string->create("Coins: %i x 10 -> %i", coins, coinsScore);
		coinsScore_label->text = string->GetString();

		enemiesScore = enemies * 10;
		string->create("Enemies: %i x 10 -> %i", enemies, enemiesScore);
		enemiesScore_label->text = string->GetString();

		timeScore = -(time - 120) * 0.1;
		if (timeScore > 0) { timeScore = 0; }
		string->create("Time: %s x -0.1 -> %i", App->ui->timer_label->text, timeScore);
		timeScore_label->text = string->GetString();

		totalScore = lifesScore + coinsScore + enemiesScore + timeScore;
		string->create("TOTAL SCORE -> %i", totalScore);
		totalScore_label->text = string->GetString();
	}
	else
	{
		score_label->enabled = false;
		timeScore_label->enabled = false;
		coinsScore_label->enabled = false;
		enemiesScore_label->enabled = false;
		lifesScore_label->enabled = false;
		totalScore_label->enabled = false;
	}

	return true;
}