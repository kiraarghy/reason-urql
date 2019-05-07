/* Opens our styles module */
open GetAllStyles;

type state = {
  listOfPokemons: Js.Array.t(string),
  textInput: string,
  filteredList: Js.Array.t(string),
  selectedPokemon: Js.Nullable.t(string),
};

type action =
  | ChangeInput(string)
  | SelectPokemon(string);

/* Filters our pokemon list by our input */
let filteredPokemonList = (~pokemonList: Js.Array.t(string), ~input: string) =>
  Js.Array.filter(
    pokemon => Js.String.includes(input, Js.String.toLowerCase(pokemon)),
    pokemonList,
  );

let component = ReasonReact.reducerComponent("GetAll");

let make = (~pokemons, _children) => {
  ...component,
  initialState: () => {
    listOfPokemons: pokemons,
    textInput: "",
    filteredList: filteredPokemonList(~pokemonList=pokemons, ~input=""),
    selectedPokemon: Js.Nullable.null,
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeInput(textInput) =>
      ReasonReact.Update({
        ...state,
        textInput,
        filteredList:
          filteredPokemonList(
            ~pokemonList=state.listOfPokemons,
            ~input=textInput,
          ),
      })
    | SelectPokemon(selected) =>
      ReasonReact.Update({
        ...state,
        selectedPokemon: Js.Nullable.return(selected),
      })
    },
  render: self => {
    let pokemonElementsArray = (~pokemonList: Js.Array.t(string)) =>
      Js.Array.map(
        pokemon =>
          <li key=pokemon className=Styles.list>
            <button
              className=Styles.listButton
              onClick={_event => self.send(SelectPokemon(pokemon))}>
              pokemon->ReasonReact.string
            </button>
          </li>,
        pokemonList,
      );
    <div className=Styles.container>
      <section className=Styles.search>
        <input
          className=Styles.searchBox
          value={self.state.textInput}
          onChange={
            event =>
              self.send(ChangeInput(event->ReactEvent.Form.target##value))
          }
        />
        <ul>
          {
            ReasonReact.array(
              pokemonElementsArray(~pokemonList=self.state.filteredList),
            )
          }
        </ul>
      </section>
      {
        switch (Js.Nullable.toOption(self.state.selectedPokemon)) {
        | Some(pokemon) => <Monster pokemon />
        | None => ReasonReact.null
        }
      }
    </div>;
  },
};